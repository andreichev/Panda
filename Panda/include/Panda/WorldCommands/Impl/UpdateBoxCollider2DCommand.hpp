#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateBoxCollider2DCommand : public WorldCommand {
public:
    UpdateBoxCollider2DCommand(const std::vector<Entity> &entities)
        : m_entities(entities)
        , m_newColliders()
        , m_prevColliders() {}

    bool undo() override {
        for (Entity entity : m_entities) {
            if (!entity.isValid()) { return false; }
            entity.setComponent(m_prevColliders[entity.getId()]);
            entity.physics2DPropertiesUpdated();
            entity.setWorldChanged();
        }
        return true;
    }

    bool execute() override {
        for (Entity entity : m_entities) {
            if (!entity.isValid()) { return false; }
            entity.setComponent(m_newColliders[entity.getId()]);
            entity.physics2DPropertiesUpdated();
            entity.setWorldChanged();
        }
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) { return false; }
        UpdateBoxCollider2DCommand &other = static_cast<UpdateBoxCollider2DCommand &>(command);
        return other.m_entities == m_entities;
    }

    void merge(WorldCommand &command) override {
        UpdateBoxCollider2DCommand &cmd = static_cast<UpdateBoxCollider2DCommand &>(command);
        m_newColliders = cmd.m_newColliders;
    }

    void saveBeforeEdit() {
        for (auto entity : m_entities) {
            m_prevColliders[entity.getId()] = entity.getComponent<BoxCollider2DComponent>();
        }
    }

    void saveAfterEdit() {
        for (auto entity : m_entities) {
            m_newColliders[entity.getId()] = entity.getComponent<BoxCollider2DComponent>();
        }
    }

private:
    std::vector<Entity> m_entities;
    std::unordered_map<UUID, BoxCollider2DComponent> m_newColliders;
    std::unordered_map<UUID, BoxCollider2DComponent> m_prevColliders;
};

} // namespace Panda