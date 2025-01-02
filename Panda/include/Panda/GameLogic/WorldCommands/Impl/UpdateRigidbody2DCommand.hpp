#pragma once

#include "Panda/GameLogic/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateRigidbody2DCommand : public WorldCommand {
public:
    UpdateRigidbody2DCommand(Entity entity, Rigidbody2DComponent newRigidbody)
        : m_entity(entity)
        , m_newRigidbody(newRigidbody)
        , m_prevRigidbody(entity.getComponent<Rigidbody2DComponent>()) {}

    bool undo() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_prevRigidbody);
        m_entity.physics2DPropertiesUpdated();
        m_entity.setWorldChanged();
        return true;
    }

    bool execute() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_newRigidbody);
        m_entity.physics2DPropertiesUpdated();
        m_entity.setWorldChanged();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        return false;
    }

    void merge(WorldCommand &command) override {
        PND_ASSERT(false, "NOT IMPLEMENTED");
    }

private:
    Entity m_entity;
    Rigidbody2DComponent m_newRigidbody;
    Rigidbody2DComponent m_prevRigidbody;
};

} // namespace Panda