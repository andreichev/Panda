//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/GameLogic/UI/UIView.hpp"

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
    UIView *getUIView();
    inline bool isRunning() {
        return m_isRunning;
    }

private:
    bool m_isRunning;
    entt::registry m_registry;
    UIView m_uiRoot;

    friend class WorldHierarchyPanel;
};

} // namespace Panda