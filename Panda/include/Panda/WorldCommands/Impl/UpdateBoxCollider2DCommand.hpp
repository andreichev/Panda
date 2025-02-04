#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateBoxCollider2DCommand : public WorldCommand {
public:
    UpdateBoxCollider2DCommand(
        const std::vector<Entity> &entities, BoxCollider2DComponent newCollider
    )
        : m_entities(entities)
        , m_newCollider(newCollider)
        , m_prevColliders() {
        for (auto entity : entities) {
            m_prevColliders[entity.getId()] = entity.getComponent<BoxCollider2DComponent>();
        }
    }

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
            entity.setComponent(m_newCollider);
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
        m_newCollider = cmd.m_newCollider;
    }

private:
    std::vector<Entity> m_entities;
    BoxCollider2DComponent m_newCollider;
    std::unordered_map<UUID, BoxCollider2DComponent> m_prevColliders;
};

} // namespace Panda