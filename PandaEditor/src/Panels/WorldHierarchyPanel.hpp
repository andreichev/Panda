//
// Created by Michael Andreichev on 16.09.2023.
//

#pragma once

#include "Panels/EntityComponents/ComponentsDraw.hpp"

#include <Panda/GameLogic/World.hpp>

namespace Panda {

class WorldHierarchyPanel final {
public:
    WorldHierarchyPanel(World *world, ComponentsDrawOutput *componentsDrawOutput);
    void onImGuiRender();
    void setWorld(World *world) {
        m_world = world;
    }

private:
    void drawEntityNode(Entity entity);
    void drawEntityCreateMenu();

    ComponentsDraw m_componentsDraw;
    World *m_world;
    Entity m_selected;
};

} // namespace Panda
