//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Window/Window.hpp"

#include <entt/entt.hpp>

namespace Panda {

class World final {
public:
    World();
    ~World();
    void initialize();
    void update(double deltaTime);
    void onImGuiRender();
    Entity instantiateEntity();
    void destroy(Entity entity);
    inline bool isRunning() {
        return m_isRunning;
    }

private:
    bool m_isRunning;
    entt::registry m_registry;

    friend class WorldHierarchyPanel;
};

} // namespace Panda