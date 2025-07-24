#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"
#include "Panda/GameLogic/Components/TransformComponent.hpp"
#include "Panda/GameLogic/SelectionContext.hpp"

namespace Panda {

class EntityTransformCommand : public WorldCommand {
public:
    EntityTransformCommand(const std::unordered_set<Entity> &entities)
        : m_entities(entities)
        , m_beforeTransforms()
        , m_afterTransforms() {}

    bool undo() override {
        PND_ASSERT(!m_entities.empty(), "WRONG TRANSFORM COMMAND DATA");
        bool selectionsUpdated = false;
        Entity first = *m_entities.begin();
        for (auto entity : m_entities) {
            if (!entity.isValid()) { return false; }
            auto &transformComponent = entity.getTransform();
            glm::mat4 prevTransform = m_beforeTransforms[entity.getId()];
            transformComponent.setTransform(prevTransform);
            entity.setWorldChanged();
            entity.physics2DPropertiesUpdated();
            selectionsUpdated |= SelectionContext::isSelected(entity);
        }
        if (selectionsUpdated) { SelectionContext::updateValues(); }
        return true;
    }

    bool execute() override {
        PND_ASSERT(!m_entities.empty(), "WRONG TRANSFORM COMMAND DATA");
        bool selectionsUpdated = false;
        Entity first = *m_entities.begin();
        for (auto entity : m_entities) {
            if (!entity.isValid()) { return false; }
            auto &transformComponent = entity.getTransform();
            glm::mat4 newTransform = m_afterTransforms[entity.getId()];
            transformComponent.setTransform(newTransform);
            entity.setWorldChanged();
            entity.physics2DPropertiesUpdated();
            selectionsUpdated |= SelectionContext::isSelected(entity);
        }
        if (selectionsUpdated) { SelectionContext::updateValues(); }
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) { return false; }
        EntityTransformCommand &other = static_cast<EntityTransformCommand &>(command);
        return other.m_entities == m_entities;
    }

    void merge(WorldCommand &command) override {
        EntityTransformCommand &cmd = static_cast<EntityTransformCommand &>(command);
        m_afterTransforms = cmd.m_afterTransforms;
    }

    void saveBeforeEdit() {
        for (auto entity : m_entities) {
            m_beforeTransforms[entity.getId()] = entity.getTransform().getLocalTransform();
        }
    }

    void saveAfterEdit() {
        for (auto entity : m_entities) {
            m_afterTransforms[entity.getId()] = entity.getTransform().getLocalTransform();
        }
    }

private:
    std::unordered_set<Entity> m_entities;
    std::unordered_map<UUID, glm::mat4> m_beforeTransforms;
    std::unordered_map<UUID, glm::mat4> m_afterTransforms;
};

} // namespace Panda