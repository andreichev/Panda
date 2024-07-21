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
    World(World &world) = delete;
    ~World();
    void initialize();
    void fillStartupData();
    void updateRuntime(double deltaTime);
    void updateSimulation(double deltaTime, glm::mat4 viewProjectionMatrix);
    void updateEditor(double deltaTime, glm::mat4 viewProjectionMatrix);
    void onImGuiRender();
    Entity instantiateEntity();
    void destroy(Entity entity);
    void clear();
    bool isChanged();
    void resetChanged();
    void setChanged();
    inline bool isRunning() {
        return m_isRunning;
    }
    Renderer2D &getRenderer2D() {
        return m_renderer2d;
    }
    Renderer3D &getRenderer3D() {
        return m_renderer3d;
    }

    Camera *findMainCamera();
    World &operator=(World &other) = delete;

private:
    void updateBasicComponents(float deltaTime);
    Entity findMainCameraEntity();
    Entity instantiateEntity(id_t id);
    void fillEntity(Entity entity);

    bool m_isRunning;
    bool m_isChanged;
    entt::registry m_registry;
    Renderer2D m_renderer2d;
    Renderer3D m_renderer3d;

    friend class WorldHierarchyPanel;
    friend class WorldMapper;
};

} // namespace Panda