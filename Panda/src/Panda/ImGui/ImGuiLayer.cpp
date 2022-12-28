#include "pndpch.hpp"
#include "ImGuiLayer.hpp"

#include <imgui.h>

#include "Panda/Application/Application.hpp"

#include <Miren/PlatformData.hpp>

// ImGui platform impl
#include "imgui_impl_panda.hpp"
// ImGui renderer impl
#include "imgui_impl_miren.hpp"

namespace Panda {

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") {}

void ImGuiLayer::onAttach() {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    float fontSize = 16.0f; // *2.0f;
    std::string fontPath = Miren::PlatformData::get()->getResourcesPath() + "fonts/ProggyTiny.ttf";
    io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize);
    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    // ImGuiStyle &style = ImGui::GetStyle();
    // if (io.ConfigFlags) {
    //     style.WindowRounding = 20.0f;
    //     style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    // }

    // setDarkThemeColors();

    // Setup Platform/Renderer bindings
    ImGui_ImplPanda_Init();
    ImGui_ImplMiren_Init();
}

void ImGuiLayer::onDetach() {
    ImGui_ImplMiren_Shutdown();
    ImGui_ImplPanda_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::onEvent(Event *event) {
    ImGui_ImplPanda_HandleEvent(event);
}

void ImGuiLayer::begin(double deltaTime) {
    ImGui_ImplPanda_NewFrame(deltaTime);
    ImGui_ImplMiren_NewFrame();
    ImGui::NewFrame();
    // ImGuizmo::BeginFrame();
}

void ImGuiLayer::end() {
    // Rendering
    ImGui::Render();
    ImGui_ImplMiren_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::setDarkThemeColors() {}

} // namespace Panda
