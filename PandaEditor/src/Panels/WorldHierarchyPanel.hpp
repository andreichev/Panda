//
// Created by Michael Andreichev on 16.09.2023.
//

#pragma once

#include <Panda/GameLogic/World.hpp>

namespace Panda {

class WorldHierarchyPanel final {
public:
    WorldHierarchyPanel(World *world);
    void onImGuiRender();
    void setWorld(World *world) {
        m_world = world;
    }

private:
    void drawEntityNode(Entity entity);

    World *m_world;
    Entity m_selected;
};

} // namespace Panda
