#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class AddRemoveEntitiesCommand : public WorldCommand {
public:
    AddRemoveEntitiesCommand(std::vector<Entity> entities)
        : m_entities(entities) {}

    AddRemoveEntitiesCommand(const AddRemoveEntitiesCommand &) = default;

    bool undo() override {
        return execute();
    }

    bool execute() override {
        for (auto entity : m_entities) {
            EditorMetadataComponent &metadata = entity.getComponent<EditorMetadataComponent>();
            metadata.isDeleted = !metadata.isDeleted;
        }
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        return false;
    }

private:
    std::vector<Entity> m_entities;
};

} // namespace Panda