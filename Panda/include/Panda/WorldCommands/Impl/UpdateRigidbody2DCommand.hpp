#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateRigidbody2DCommand : public WorldCommand {
public:
    UpdateRigidbody2DCommand(const std::vector<Entity> &entities, Rigidbody2DComponent newRigidbody)
        : m_entities(entities)
        , m_newRigidbody(newRigidbody)
        , m_prevRigidbodies() {
        for (auto entity : entities) {
            m_prevRigidbodies[entity.getId()] = entity.getComponent<Rigidbody2DComponent>();
        }
    }

    bool undo() override {
        for (Entity entity : m_entities) {
            if (!entity.isValid()) { return false; }
            entity.setComponent(m_prevRigidbodies[entity.getId()]);
            entity.physics2DPropertiesUpdated();
            entity.setWorldChanged();
        }
        return true;
    }

    bool execute() override {
        for (Entity entity : m_entities) {
            if (!entity.isValid()) { return false; }
            entity.setComponent(m_newRigidbody);
            entity.physics2DPropertiesUpdated();
            entity.setWorldChanged();
        }
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        return false;
    }

    void merge(WorldCommand &command) override {
        PND_ASSERT(false, "NOT IMPLEMENTED");
    }

private:
    std::vector<Entity> m_entities;
    Rigidbody2DComponent m_newRigidbody;
    std::unordered_map<UUID, Rigidbody2DComponent> m_prevRigidbodies;
};

} // namespace Panda