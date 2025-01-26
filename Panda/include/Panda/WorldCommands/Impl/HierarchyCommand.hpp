#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class HierarchyCommand : public WorldCommand {
public:
    HierarchyCommand(Entity entity, Entity newParent)
        : m_entity(entity)
        , m_newParent(newParent) {
        m_prevParent = entity.getParent();
    }

    bool undo() override {
        if (m_prevParent) {
            m_prevParent.addChildEntity(m_entity);
        } else {
            m_entity.removeFromParent();
        }
        m_entity.sortWorld();
        m_entity.setWorldChanged();
        return true;
    }

    bool execute() override {
        if (m_newParent) {
            m_newParent.addChildEntity(m_entity);
        } else {
            m_entity.removeFromParent();
        }
        m_entity.sortWorld();
        m_entity.setWorldChanged();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        return false;
    }

private:
    Entity m_entity;
    Entity m_newParent;
    Entity m_prevParent;
};

} // namespace Panda