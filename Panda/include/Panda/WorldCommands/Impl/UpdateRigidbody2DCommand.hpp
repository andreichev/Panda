#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateRigidbody2DCommand : public WorldCommand {
public:
    UpdateRigidbody2DCommand(const std::vector<Entity> &entities)
        : m_entities(entities)
        , m_newRigidbodies()
        , m_prevRigidbodies() {}

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
            entity.setComponent(m_newRigidbodies[entity.getId()]);
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

    void saveBeforeEdit() {
        for (auto entity : m_entities) {
            m_prevRigidbodies[entity.getId()] = entity.getComponent<Rigidbody2DComponent>();
        }
    }

    void saveAfterEdit() {
        for (auto entity : m_entities) {
            m_newRigidbodies[entity.getId()] = entity.getComponent<Rigidbody2DComponent>();
        }
    }

private:
    std::vector<Entity> m_entities;
    std::unordered_map<UUID, Rigidbody2DComponent> m_newRigidbodies;
    std::unordered_map<UUID, Rigidbody2DComponent> m_prevRigidbodies;
};

} // namespace Panda