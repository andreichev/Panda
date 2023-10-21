#pragma once

#include <Panda.hpp>

#include <imgui.h>

namespace Panda {

class StatisticsPanel final {
public:
    static void onImGuiRender() {
        ImGui::Begin("Renderer2D statistics");
        ImGui::Text("FPS: %d", Application::get()->fps);
        auto stats = Application::get()->getRenderer2D().getStats();
        ImGui::Text("Quads count: %d", stats.quadCount);
        ImGui::Text("Vertices count: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices count: %d", stats.getTotalIndexCount());
        ImGui::Text("Draw calls: %d", stats.drawCalls);
        ImGui::End();
    }
};

} // namespace Panda