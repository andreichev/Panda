#pragma once
#include <imgui.h> // IMGUI_IMPL_API

// Backend API
IMGUI_IMPL_API bool ImGui_ImplMiren_Init();
IMGUI_IMPL_API void ImGui_ImplMiren_Shutdown();
IMGUI_IMPL_API void ImGui_ImplMiren_NewFrame();
IMGUI_IMPL_API void ImGui_ImplMiren_RenderDrawData(ImDrawData *draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API bool ImGui_ImplMiren_CreateFontsTexture();
IMGUI_IMPL_API void ImGui_ImplMiren_DestroyFontsTexture();
IMGUI_IMPL_API bool ImGui_ImplMiren_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplMiren_DestroyDeviceObjects();
