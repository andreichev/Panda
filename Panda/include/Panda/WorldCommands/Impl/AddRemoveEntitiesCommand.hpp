#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class AddRemoveEntitiesCommand : public WorldCommand {
public:
    AddRemoveEntitiesCommand(const std::unordered_set<Entity> &entities)
        : m_entities(entities) {}

    AddRemoveEntitiesCommand(AddRemoveEntitiesCommand &&other)
        : m_entities(std::move(other.m_entities)) {}

    ~AddRemoveEntitiesCommand() {
        if (m_entities.empty()) { return; }
        for (auto entity : m_entities) {
            if (entity.needToDestroy()) {
                auto world = entity.getWorld();
                world->destroy(entity);
            }
        }
    }

    bool undo() override {
        return execute();
    }

    bool execute() override {
        if (m_entities.empty()) { return false; }
        Entity firstEntity = *m_entities.begin();
        World *world = firstEntity.getWorld();
        SelectionContext &selectionContext = world->getSelectionContext();
        for (auto entity : m_entities) {
            EditorMetadataComponent &metadata = entity.getComponent<EditorMetadataComponent>();
            metadata.isDeleted = !metadata.isDeleted;
            if (metadata.isDeleted && selectionContext.isSelected(entity)) {
                selectionContext.removeSelectedEntity(entity);
            }
        }
        firstEntity.setWorldChanged();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        return false;
    }

private:
    std::unordered_set<Entity> m_entities;
};

} // namespace Panda