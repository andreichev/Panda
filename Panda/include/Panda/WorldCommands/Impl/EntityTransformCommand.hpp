#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"
#include "Panda/GameLogic/Components/TransformComponent.hpp"

namespace Panda {

class EntityTransformCommand : public WorldCommand {
public:
    EntityTransformCommand(std::vector<Entity> entities)
        : m_entities(entities)
        , m_beforeTransforms()
        , m_afterTransforms() {}

    bool undo() override {
        PND_ASSERT(!m_entities.empty(), "WRONG TRANSFORM COMMAND DATA");
        bool selectionsUpdated = false;
        SelectionContext &selectionContext = m_entities[0].getWorld()->getSelectionContext();
        for (auto entity : m_entities) {
            if (!entity.isValid()) {
                return false;
            }
            auto &transformComponent = entity.getTransform();
            glm::mat4 prevTransform = m_beforeTransforms[entity.getId()];
            transformComponent.setTransform(prevTransform);
            entity.setWorldChanged();
            entity.physics2DPropertiesUpdated();
            selectionsUpdated |= selectionContext.isSelected(entity);
        }
        if (selectionsUpdated) {
            selectionContext.updateValues();
        }
        return true;
    }

    bool execute() override {
        PND_ASSERT(!m_entities.empty(), "WRONG TRANSFORM COMMAND DATA");
        bool selectionsUpdated = false;
        SelectionContext &selectionContext = m_entities[0].getWorld()->getSelectionContext();
        for (auto entity : m_entities) {
            if (!entity.isValid()) {
                return false;
            }
            auto &transformComponent = entity.getTransform();
            glm::mat4 newTransform = m_afterTransforms[entity.getId()];
            transformComponent.setTransform(newTransform);
            entity.setWorldChanged();
            entity.physics2DPropertiesUpdated();
            selectionsUpdated |= selectionContext.isSelected(entity);
        }
        if (selectionsUpdated) {
            selectionContext.updateValues();
        }
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) {
            return false;
        }
        EntityTransformCommand &other = static_cast<EntityTransformCommand &>(command);
        return other.m_entities == m_entities;
    }

    void merge(WorldCommand &command) override {
        EntityTransformCommand &cmd = static_cast<EntityTransformCommand &>(command);
        m_afterTransforms = cmd.m_afterTransforms;
    }

    void saveBeforeTransforms() {
        for (auto entity : m_entities) {
            m_beforeTransforms[entity.getId()] = entity.getTransform().getLocalTransform();
        }
    }

    void saveAfterTransforms() {
        for (auto entity : m_entities) {
            m_afterTransforms[entity.getId()] = entity.getTransform().getLocalTransform();
        }
    }

private:
    std::vector<Entity> m_entities;
    std::unordered_map<UUID, glm::mat4> m_beforeTransforms;
    std::unordered_map<UUID, glm::mat4> m_afterTransforms;
};

} // namespace Panda