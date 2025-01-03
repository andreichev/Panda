#pragma once

#include "Panda/GameLogic/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"
#include "Panda/GameLogic/Components/TransformComponent.hpp"

namespace Panda {

class EntityTransformCommand : public WorldCommand {
public:
    EntityTransformCommand(Entity entity, TransformComponent newTransform)
        : m_entity(entity)
        , m_newTransform(newTransform)
        , m_prevTransform(entity.getTransform()) {}

    bool undo() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_prevTransform);
        m_entity.setWorldChanged();
        m_entity.physics2DPropertiesUpdated();
        return true;
    }

    bool execute() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_newTransform);
        m_entity.setWorldChanged();
        m_entity.physics2DPropertiesUpdated();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) {
            return false;
        }
        EntityTransformCommand &other = static_cast<EntityTransformCommand &>(command);
        return other.m_entity == m_entity;
    }

    void merge(WorldCommand &command) override {
        EntityTransformCommand &cmd = static_cast<EntityTransformCommand &>(command);
        m_newTransform = cmd.m_newTransform;
    }

private:
    Entity m_entity;
    TransformComponent m_newTransform;
    TransformComponent m_prevTransform;
};

} // namespace Panda