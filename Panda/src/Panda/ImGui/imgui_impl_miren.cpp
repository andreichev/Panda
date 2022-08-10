#include "imgui_impl_miren.hpp"

#include "Panda/Renderer/Miren.hpp"

#include <glm/glm.hpp>

bool deviceObjectsCreated = false;
Panda::ShaderHandle shader;
Panda::VertexBufferHandle vertexBuffer;
Panda::IndexBufferHandle indexBuffer;
Panda::TextureHandle fontTexture;
glm::mat4 projMat(1.f);

IMGUI_IMPL_API bool ImGui_ImplMiren_Init() {
    ImGuiIO &io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_miren";
}

IMGUI_IMPL_API void ImGui_ImplMiren_Shutdown() {
    ImGui_ImplMiren_DestroyDeviceObjects();
}

IMGUI_IMPL_API void ImGui_ImplMiren_NewFrame() {
    if (deviceObjectsCreated == false) {
        ImGui_ImplMiren_CreateDeviceObjects();
        deviceObjectsCreated = true;
    }
}

static void ImGui_ImplMiren_SetProjMat(ImDrawData *draw_data, int fb_width, int fb_height) {
    using namespace Panda;

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    projMat = glm::ortho(L, R, B, T);
    Miren::setUniform(shader, "Texture", (void *)0, UniformDataType::Int);
    Miren::setUniform(shader, "ProjMtx", &projMat[0], UniformDataType::Mat4);
}

IMGUI_IMPL_API void ImGui_ImplMiren_RenderDrawData(ImDrawData *draw_data) {
    using namespace Panda;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;
    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        // Upload vertex/index buffers
        void *vertices = malloc(cmd_list->VtxBuffer.size_in_bytes());
        memcpy(vertices, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.size_in_bytes());
        Panda::Miren::updateDynamicVertexBuffer(vertexBuffer, vertices, cmd_list->VtxBuffer.size_in_bytes());

        void *indices = malloc(cmd_list->IdxBuffer.size_in_bytes());
        memcpy(indices, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.size_in_bytes());
        Panda::Miren::updateDynamicIndexBuffer(indexBuffer, indices, cmd_list->IdxBuffer.Size);
        const ImDrawCmd *cmd = cmd_list->CmdBuffer.begin(), *cmdEnd = cmd_list->CmdBuffer.end();
        
        const ImVec2 clipPos   = draw_data->DisplayPos;
		const ImVec2 clipScale = draw_data->FramebufferScale;
        
        for (; cmd != cmdEnd; ++cmd) {
            if (cmd->UserCallback) {
                cmd->UserCallback(cmd_list, cmd);
            } else if (0 != cmd->ElemCount) {
                ImVec4 clipRect;
				clipRect.x = (cmd->ClipRect.x - clipPos.x) * clipScale.x;
				clipRect.y = (cmd->ClipRect.y - clipPos.y) * clipScale.y;
				clipRect.z = (cmd->ClipRect.z - clipPos.x) * clipScale.x;
				clipRect.w = (cmd->ClipRect.w - clipPos.y) * clipScale.y;

                if (clipRect.x <  fb_width
					&&  clipRect.y <  fb_height
					&&  clipRect.z >= 0.0f
					&&  clipRect.w >= 0.0f)
                {
					// Miren::setScissorRect(
                    //     UIRect(
                    //         uint32_t(clipRect.x), 
                    //         uint32_t(fb_height - clipRect.w),
                    //         uint32_t(clipRect.z - clipRect.x),
                    //         uint32_t(clipRect.w - clipRect.y)
                    //     )
					// );

                    Miren::setState(0);
                    Miren::setShader(shader);
                    ImGui_ImplMiren_SetProjMat(draw_data, fb_width, fb_height);
                    TextureHandle texture = (TextureHandle)(intptr_t)cmd->GetTexID();
                    Miren::setTexture(texture, 0);
                    Miren::setVertexBuffer(vertexBuffer);
                    Miren::setIndexBuffer(indexBuffer, (void *)(intptr_t)(cmd->IdxOffset * sizeof(ImDrawIdx)), cmd->ElemCount);
                    Miren::submit();
                }
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

    using namespace Panda;
    fontTexture = Miren::createTextureFromPixels(texture, width, height);
    io.Fonts->SetTexID((ImTextureID)fontTexture);
    return true;
}

IMGUI_IMPL_API void ImGui_ImplMiren_DestroyFontsTexture() {
    Panda::Miren::deleteTexture(fontTexture);
}

IMGUI_IMPL_API bool ImGui_ImplMiren_CreateDeviceObjects() {
    using namespace Panda;
    shader = Miren::createShader("shaders/imgui/imgui_vertex.glsl", "shaders/imgui/imgui_fragment.glsl");

    VertexBufferLayoutData layoutData;
    layoutData.pushVec2();
    layoutData.pushVec2();
    layoutData.push8BitRGBAColor();
    VertexLayoutHandle vertexLayout = Miren::createVertexLayout(layoutData);

    // 1 MB vertex buffer
    size_t vb_size_in_bytes = 1000 * 1000;
    void *vertices = malloc(vb_size_in_bytes);
    memset(vertices, 0, vb_size_in_bytes);
    vertexBuffer = Miren::createDynamicVertexBuffer(vertices, vb_size_in_bytes, vertexLayout);

    ImGui_ImplMiren_CreateFontsTexture();

    // 1 MB index buffer
    size_t ib_size_in_bytes = 1000 * 1000;
    size_t ib_count = ib_size_in_bytes / 2;
    void *indices = malloc(ib_size_in_bytes);
    memset(indices, 0, ib_size_in_bytes);
    indexBuffer = Miren::createDynamicIndexBuffer(indices, BufferElementType::UnsignedShort, ib_count);
}

IMGUI_IMPL_API void ImGui_ImplMiren_DestroyDeviceObjects() {
    using namespace Panda;
    Miren::deleteShader(shader);
    Miren::deleteVertexBuffer(vertexBuffer);
    Miren::deleteIndexBuffer(indexBuffer);
    ImGui_ImplMiren_DestroyFontsTexture();
    deviceObjectsCreated = false;
}
