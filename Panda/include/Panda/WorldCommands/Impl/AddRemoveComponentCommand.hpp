#pragma once

#include "Panda/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

template<typename T>
class AddRemoveComponentCommand : public WorldCommand {
public:
    AddRemoveComponentCommand(const std::unordered_set<Entity> &entities)
        : m_entities(entities) {}

    bool undo() override {
        return execute();
    }

    bool execute() override {
        bool allEntitiesHaveComponent = true;
        for (Entity entity : m_entities) {
            if (!entity.hasComponent<T>()) {
                allEntitiesHaveComponent = false;
                break;
            }
        }
        for (Entity entity : m_entities) {
            if (!entity.isValid()) { return false; }
            if (allEntitiesHaveComponent) {
                component = entity.getComponent<T>();
                entity.removeComponent<T>();
            } else if (!entity.hasComponent<T>()) {
                entity.addComponent<T>(component);
            }
            entity.setWorldChanged();
        }
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) { return false; }
        AddRemoveComponentCommand &other = static_cast<AddRemoveComponentCommand &>(command);
        return other.m_entities == m_entities;
    }

private:
    std::unordered_set<Entity> m_entities;
    T component;
};

} // namespace Panda