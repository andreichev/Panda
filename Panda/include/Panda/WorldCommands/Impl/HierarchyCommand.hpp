#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class HierarchyCommand : public WorldCommand {
public:
    HierarchyCommand(const std::vector<Entity> &entities, Entity newParent)
        : m_entities(entities)
        , m_newParent(newParent) {
        PND_ASSERT(!entities.empty(), "Entities should not be empty");
        for (auto entity : entities) {
            m_prevParents[entity.getId()] = entity.getParent();
        }
    }

    bool undo() override {
        for (auto entity : m_entities) {
            UUID entityId = entity.getId();
            if (m_prevParents.at(entityId).isValid()) {
                m_prevParents.at(entityId).addChildEntity(entity);
            } else {
                entity.removeFromParent();
            }
        }
        m_entities[0].sortWorld();
        m_entities[0].setWorldChanged();
        return true;
    }

    bool execute() override {
        for (auto entity : m_entities) {
            if (m_newParent) {
                m_newParent.addChildEntity(entity);
            } else {
                entity.removeFromParent();
            }
        }
        m_entities[0].sortWorld();
        m_entities[0].setWorldChanged();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        return false;
    }

private:
    std::vector<Entity> m_entities;
    Entity m_newParent;
    std::unordered_map<UUID, Entity> m_prevParents;
};

} // namespace Panda