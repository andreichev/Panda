//
// Created by Michael Andreichev on 20.12.2023.
//

#pragma once

#include <Panda/GameLogic/World.hpp>

namespace Panda {

class ComponentsDrawOutput {
public:
    virtual ~ComponentsDrawOutput() = default;
    virtual void addScriptToEntities(const std::vector<Entity> &entities) = 0;
};

class ComponentsDraw {
public:
    ComponentsDraw(ComponentsDrawOutput *output);
    void drawComponents(const std::vector<Entity> &entities);

private:
    void displayAddScriptMenuItem(const std::vector<Entity> &entities);

    ComponentsDrawOutput *m_output;
};

} // namespace Panda