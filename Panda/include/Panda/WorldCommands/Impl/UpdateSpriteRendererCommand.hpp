#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class UpdateSpriteRendererCommand : public WorldCommand {
public:
    UpdateSpriteRendererCommand(const std::vector<Entity> &entities)
        : m_entities(entities)
        , m_newSprites()
        , m_prevSprites() {}

    bool undo() override {
        for (Entity entity : m_entities) {
            if (!entity.isValid()) { return false; }
            entity.setComponent(m_prevSprites[entity.getId()]);
            entity.setWorldChanged();
        }
        return true;
    }

    bool execute() override {
        for (Entity entity : m_entities) {
            if (!entity.isValid()) { return false; }
            entity.setComponent(m_newSprites[entity.getId()]);
            entity.setWorldChanged();
        }
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) { return false; }
        UpdateSpriteRendererCommand &other = static_cast<UpdateSpriteRendererCommand &>(command);
        return other.m_entities == m_entities;
    }

    void merge(WorldCommand &command) override {
        UpdateSpriteRendererCommand &cmd = static_cast<UpdateSpriteRendererCommand &>(command);
        m_newSprites = cmd.m_newSprites;
    }

    void saveBeforeEdit() {
        for (auto entity : m_entities) {
            m_prevSprites[entity.getId()] = entity.getComponent<SpriteRendererComponent>();
        }
    }

    void saveAfterEdit() {
        for (auto entity : m_entities) {
            m_newSprites[entity.getId()] = entity.getComponent<SpriteRendererComponent>();
        }
    }

private:
    std::vector<Entity> m_entities;
    std::unordered_map<UUID, SpriteRendererComponent> m_newSprites;
    std::unordered_map<UUID, SpriteRendererComponent> m_prevSprites;
};

} // namespace Panda