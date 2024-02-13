//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Renderer/Renderer2D.hpp"
#include "Panda/Renderer/Renderer3D.hpp"

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
    Renderer2D &getRenderer2D() {
        return m_renderer2d;
    }
    Renderer3D &getRenderer3D() {
        return m_renderer3d;
    }

private:
    Entity getMainCameraEntity();

    bool m_isRunning;
    entt::registry m_registry;
    Renderer2D m_renderer2d;
    Renderer3D m_renderer3d;

    friend class WorldHierarchyPanel;
};

} // namespace Panda