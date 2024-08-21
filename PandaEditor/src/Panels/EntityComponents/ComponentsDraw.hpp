//
// Created by Michael Andreichev on 20.12.2023.
//

#pragma once

#include <Panda/GameLogic/World.hpp>

namespace Panda {

class ComponentsDrawOutput {
public:
    virtual ~ComponentsDrawOutput() = default;
    virtual void addScriptToEntity(Entity entity) = 0;
};

class ComponentsDraw {
public:
    ComponentsDraw(ComponentsDrawOutput *output);
    void drawComponents(Entity entity);

private:
    void displayAddScriptMenuItem(Entity entity);

    ComponentsDrawOutput *m_output;
};

} // namespace Panda