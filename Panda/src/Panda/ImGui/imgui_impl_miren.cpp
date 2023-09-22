#include "imgui_impl_miren.hpp"
#include "Panda/Assets/AssetLoader.hpp"

#include <Miren/Miren.hpp>

#include <glm/glm.hpp>

bool deviceObjectsCreated = false;
Miren::ProgramHandle shader;
Miren::VertexLayoutHandle vertexLayout;
Miren::TextureHandle fontTexture;
glm::mat4 projMat(1.f);

IMGUI_IMPL_API bool ImGui_ImplMiren_Init() {
    ImGuiIO &io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_miren";
    // io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
    return true;
}

IMGUI_IMPL_API void ImGui_ImplMiren_Shutdown() {
    ImGui_ImplMiren_DestroyDeviceObjects();
}

IMGUI_IMPL_API void ImGui_ImplMiren_NewFrame() {
    if (!deviceObjectsCreated) {
        ImGui_ImplMiren_CreateDeviceObjects();
        deviceObjectsCreated = true;
    }
}

static void ImGui_ImplMiren_SetProjMat(ImDrawData *draw_data, int fb_width, int fb_height) {
    using namespace Miren;

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    projMat = glm::ortho(L, R, B, T);
    Miren::setUniform(shader, "Texture", (void *)0, UniformDataType::Int);
    Miren::setUniform(shader, "ProjMtx", &projMat[0], UniformDataType::Mat4);
}

IMGUI_IMPL_API void ImGui_ImplMiren_RenderDrawData(ImDrawData *draw_data) {
    using namespace Miren;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates !=
    // framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0) {
        return;
    }

    ImGui_ImplMiren_SetProjMat(draw_data, fb_width, fb_height);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];

        // Vertex/index buffers data
        Miren::TransientVertexBuffer tvb;
        Miren::allocTransientVertexBuffer(&tvb, cmd_list->VtxBuffer.size_in_bytes());
        memcpy(tvb.data, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.size_in_bytes());

        Miren::TransientIndexBuffer tib;
        Miren::allocTransientIndexBuffer(
            &tib, cmd_list->IdxBuffer.size(), Miren::BufferElementType::UnsignedShort);
        memcpy(tib.data, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.size_in_bytes());

        // Will project scissor/clipping rectangles into framebuffer space
        ImVec2 clip_off = draw_data->DisplayPos; // (0,0) unless using multi-viewports
        ImVec2 clip_scale =
            draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

        const ImDrawCmd *cmd = cmd_list->CmdBuffer.begin();
        const ImDrawCmd *cmdEnd = cmd_list->CmdBuffer.end();
        for (; cmd != cmdEnd; ++cmd) {
            if (cmd->UserCallback != NULL) {
                cmd->UserCallback(cmd_list, cmd);
            } else {
                ImVec2 clip_min((cmd->ClipRect.x - clip_off.x) * clip_scale.x,
                    (cmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((cmd->ClipRect.z - clip_off.x) * clip_scale.x,
                    (cmd->ClipRect.w - clip_off.y) * clip_scale.y);
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) {
                    continue;
                }
                Rect scissorRect = Rect(clip_min.x,
                    ((float)fb_height - clip_max.y),
                    (clip_max.x - clip_min.x),
                    (clip_max.y - clip_min.y));
                Miren::setScissorRect(scissorRect);
                Miren::setState(0);
                Miren::setShader(shader);
                TextureHandle texture = TextureHandle((intptr_t)cmd->GetTexID());
                Miren::setTexture(texture, 0);
                Miren::setVertexBuffer(tvb.handle, tvb.startVertex);
                Miren::setVertexLayout(vertexLayout);
                uint32_t offset = cmd->IdxOffset * sizeof(ImDrawIdx);
                Miren::setIndexBuffer(tib.handle, tib.startIndex + offset, cmd->ElemCount);
                Miren::submit(0);
            }
        }
    }
}

IMGUI_IMPL_API bool ImGui_ImplMiren_CreateFontsTexture() {
    // Build texture atlas
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    uint32_t textureSize = width * height * 4;
    void *texture = malloc(textureSize);
    memcpy(texture, pixels, textureSize);

    using namespace Miren;
    TextureCreate create;
    create.m_data =
        Foundation::Memory(texture, nullptr, [](void *ptr, void *userData) { free(ptr); });
    create.m_format = TextureFormat::RGBA8;
    create.m_width = width;
    create.m_height = height;
    create.m_numMips = 0;
    fontTexture = createTexture(create);
    io.Fonts->SetTexID((ImTextureID)(intptr_t)fontTexture.id);
    return true;
}

IMGUI_IMPL_API void ImGui_ImplMiren_DestroyFontsTexture() {
    Miren::deleteTexture(fontTexture);
}

IMGUI_IMPL_API bool ImGui_ImplMiren_CreateDeviceObjects() {
    using namespace Miren;
    Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
        "default-shaders/imgui/imgui_vertex.glsl", "default-shaders/imgui/imgui_fragment.glsl");
    shader = Miren::createProgram(programAsset.getMirenProgramCreate());

    VertexBufferLayoutData layoutData;
    layoutData.pushVec2();
    layoutData.pushVec2();
    layoutData.push8BitRGBAColor();
    vertexLayout = Miren::createVertexLayout(layoutData);
    ImGui_ImplMiren_CreateFontsTexture();
    return true;
}

IMGUI_IMPL_API void ImGui_ImplMiren_DestroyDeviceObjects() {
    using namespace Miren;
    deleteProgram(shader);
    ImGui_ImplMiren_DestroyFontsTexture();
    deviceObjectsCreated = false;
}
