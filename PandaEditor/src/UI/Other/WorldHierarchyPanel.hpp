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
    bool isFocused() {
        return m_focused;
    }

private:
    void drawEntityNode(Entity entity);
    void drawEntityCreateMenu();

    bool m_focused;
    int m_rowIndex;
    int m_firstSelectedRow;
    int m_lastSelectedRow;
    bool m_shiftSelectionRunning;
    World *m_world;
};

} // namespace Panda
