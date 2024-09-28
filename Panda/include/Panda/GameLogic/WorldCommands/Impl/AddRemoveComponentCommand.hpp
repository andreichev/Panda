#pragma once

#include "Panda/GameLogic/WorldCommands/WorldCommand.hpp"
#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

template<typename T>
class AddRemoveComponentCommand : public WorldCommand {
public:
    AddRemoveComponentCommand(Entity entity)
        : m_entity(entity) {}

    bool undo() override {
        return execute();
    }

    bool execute() override {
        if (!m_entity.isValid()) {
            return false;
        }
        if (m_entity.hasComponent<T>()) {
            component = m_entity.getComponent<T>();
            m_entity.removeComponent<T>();
        } else {
            m_entity.addComponent<T>(component);
        }
        m_entity.setWorldChanged();
        return true;
    }

    bool canMerge(WorldCommand &command) override {
        if (typeid(command) != typeid(*this)) {
            return false;
        }
        AddRemoveComponentCommand &other = static_cast<AddRemoveComponentCommand &>(command);
        return other.m_entity == m_entity;
    }

private:
    Entity m_entity;
    T component;
};

} // namespace Panda