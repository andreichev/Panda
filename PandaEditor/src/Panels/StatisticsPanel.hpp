#pragma once

#include <Panda.hpp>

#include <imgui.h>

namespace Panda {

class StatisticsPanel final {
public:
    StatisticsPanel(World *world)
        : m_world(world) {}

    void onImGuiRender() {
        ImGui::Begin("Renderer2D statistics");
        ImGui::Text("FPS: %d", Application::get()->fps);
        if (m_world == nullptr) {
            ImGui::End();
            return;
        }
        auto stats = m_world->getRenderer2D().getStats();
        ImGui::Text("Quads count: %d", stats.quadCount);
        ImGui::Text("Vertices count: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices count: %d", stats.getTotalIndexCount());
        ImGui::Text("Draw calls: %d", stats.drawCalls);
        ImGui::End();
    }

    void setWorld(World *world) {
        m_world = world;
    }

private:
    World *m_world;
};

} // namespace Panda