#pragma once

#include <imgui.h> // IMGUI_IMPL_API

#include <Fern/Events/Event.hpp>
#include <Fern/Window/Window.hpp>

// Backend API
IMGUI_IMPL_API bool ImGui_ImplPanda_Init();
IMGUI_IMPL_API void ImGui_ImplPanda_Shutdown();
IMGUI_IMPL_API void ImGui_ImplPanda_HandleEvent(Fern::Event *event);
IMGUI_IMPL_API void ImGui_ImplPanda_NewFrame(Fern::Window *window, double deltaTime);
