#include "imgui_impl_miren.hpp"

IMGUI_IMPL_API bool ImGui_ImplMiren_Init() {
    ImGuiIO &io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_miren";
}

IMGUI_IMPL_API void ImGui_ImplMiren_Shutdown() {}

IMGUI_IMPL_API void ImGui_ImplMiren_NewFrame() {}

IMGUI_IMPL_API void ImGui_ImplMiren_RenderDrawData(ImDrawData *draw_data) {}

IMGUI_IMPL_API bool ImGui_ImplMiren_CreateFontsTexture() {}

IMGUI_IMPL_API void ImGui_ImplMiren_DestroyFontsTexture() {}

IMGUI_IMPL_API bool ImGui_ImplMiren_CreateDeviceObjects() {}

IMGUI_IMPL_API void ImGui_ImplMiren_DestroyDeviceObjects() {}