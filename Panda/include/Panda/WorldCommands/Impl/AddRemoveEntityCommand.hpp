#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class AddRemoveEntityCommand : public WorldCommand {
public:
    AddRemoveEntityCommand(Entity entity)
        : m_entity(entity) {}

    bool undo() override {
        return execute();
    }

    bool execute() override {
        EditorMetadataComponent &metadata = m_entity.getComponent<EditorMetadataComponent>();
        metadata.isDeleted = !metadata.isDeleted;
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        return false;
    }

private:
    Entity m_entity;
};

} // namespace Panda