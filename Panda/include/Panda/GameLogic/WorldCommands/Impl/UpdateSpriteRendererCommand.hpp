#pragma once

#include "Panda/GameLogic/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateSpriteRendererCommand : public WorldCommand {
public:
    UpdateSpriteRendererCommand(Entity entity, SpriteRendererComponent newSprite)
        : m_entity(entity)
        , m_newSprite(newSprite)
        , m_prevSprite(entity.getComponent<SpriteRendererComponent>()) {}

    bool undo() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_prevSprite);
        m_entity.setWorldChanged();
        return true;
    }

    bool execute() override {
        if (!m_entity.isValid()) {
            return false;
        }
        m_entity.setComponent(m_newSprite);
        m_entity.setWorldChanged();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) {
            return false;
        }
        UpdateSpriteRendererCommand &other = static_cast<UpdateSpriteRendererCommand &>(command);
        return other.m_entity == m_entity;
    }

    void merge(WorldCommand &command) override {
        UpdateSpriteRendererCommand &cmd = static_cast<UpdateSpriteRendererCommand &>(command);
        m_newSprite = cmd.m_newSprite;
    }

private:
    Entity m_entity;
    SpriteRendererComponent m_newSprite;
    SpriteRendererComponent m_prevSprite;
};

} // namespace Panda