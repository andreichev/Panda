//
// Created by Michael Andreichev on 20.12.2023.
//

#pragma once

#include <Panda/GameLogic/World.hpp>

namespace Panda {

class ComponentsDrawOutput {
public:
    virtual ~ComponentsDrawOutput() = default;
    virtual void addScriptToEntities(const std::unordered_set<Entity> &entities) = 0;
};

class ComponentsDraw {
public:
    ComponentsDraw(ComponentsDrawOutput *output);
    void drawComponents(const std::unordered_set<Entity> &entities);

private:
    void displayAddScriptMenuItem(const std::unordered_set<Entity> &entities);

    ComponentsDrawOutput *m_output;
};

} // namespace Panda