//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Renderer/Renderer2D.hpp"
#include "Panda/Renderer/Renderer3D.hpp"
#include "Panda/GameLogic/WorldCommands/WorldCommandManager.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class b2World;

class World final {
public:
    World();
    World(World &world) = delete;
    ~World();
    void fillStartupData();
    void startRunning();
    void finishRunning();
    void updateRuntime(double deltaTime);
    void updateSimulation(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx);
    void updateEditor(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx);
    Entity instantiateEntity();
    void rebindScriptsAndFields();
    void destroy(Entity entity);
    void clear();
    bool isEmpty();
    Entity findByTag(const char *tag);
    Entity getById(UUID id);
    Entity getByEnttId(entt::entity id);
    inline bool isRunning() {
        return m_isRunning;
    }
    Renderer2D &getRenderer2D() {
        return m_renderer2d;
    }
    Renderer3D &getRenderer3D() {
        return m_renderer3d;
    }
    void setViewId(Miren::ViewId id);
    Entity findMainCameraEntity();
    Camera *findMainCamera();

    template<typename... Components>
    auto getAllEntitiesWith() {
        return m_registry.view<Components...>();
    }

    World &operator=(World &other);
    void debugPrint();

    /// --------- Editor stuff -----------------

    inline WorldCommandManager &getCommandManger() {
        return m_commandManager;
    }

    bool isChanged();
    void setChanged(bool changed = true);

    Entity getSelectedEntity() {
        return m_selectedEntity;
    }

    void setSelectedEntity(Entity entity) {
        m_selectedEntity = entity;
    }

private:
    void updateBasicComponents(float deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx);
    Entity instantiateEntity(UUID id);
    void fillEntity(Entity entity, UUID id);

    std::unordered_map<UUID, Entity> m_entityIdMap;
    bool m_isRunning;
    entt::registry m_registry;
    b2World* m_physicsWorld = nullptr;
    Renderer2D m_renderer2d;
    Renderer3D m_renderer3d;
    Miren::ViewId m_renderingViewId;

    /// -------- Editor stuff -----------
    bool m_isChanged;
    WorldCommandManager m_commandManager;
    Entity m_selectedEntity;

    friend class Entity;
    friend class WorldHierarchyPanel;
    friend class WorldMapper;
};

} // namespace Panda