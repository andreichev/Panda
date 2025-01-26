#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateBoxCollider2DCommand : public WorldCommand {
public:
    UpdateBoxCollider2DCommand(Entity entity, BoxCollider2DComponent newCollider)
        : m_entity(entity)
        , m_newCollider(newCollider)
        , m_prevCollider(entity.getComponent<BoxCollider2DComponent>()) {}

    bool undo() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_prevCollider);
        m_entity.physics2DPropertiesUpdated();
        m_entity.setWorldChanged();
        return true;
    }

    bool execute() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_newCollider);
        m_entity.physics2DPropertiesUpdated();
        m_entity.setWorldChanged();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) {
            return false;
        }
        UpdateBoxCollider2DCommand &other = static_cast<UpdateBoxCollider2DCommand &>(command);
        return other.m_entity == m_entity;
    }

    void merge(WorldCommand &command) override {
        UpdateBoxCollider2DCommand &cmd = static_cast<UpdateBoxCollider2DCommand &>(command);
        m_newCollider = cmd.m_newCollider;
    }

private:
    Entity m_entity;
    BoxCollider2DComponent m_newCollider;
    BoxCollider2DComponent m_prevCollider;
};

} // namespace Panda