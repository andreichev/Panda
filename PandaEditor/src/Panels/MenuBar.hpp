//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include "ProjectLoader/ProjectLoader.hpp"

#include <imgui.h>

namespace Panda {

class MenuBarOutput {
public:
    virtual ~MenuBarOutput() = default;
    virtual void menuBarOpenProject() = 0;
    virtual void menuBarSaveWorld() = 0;
    virtual void menuBarCloseProject() = 0;
    virtual void menuBarCloseApp() = 0;
};

class MenuBar final {
public:
    MenuBar(MenuBarOutput *output);
    void onImGuiRender();

    float m_height;
    MenuBarOutput *m_output;
};

} // namespace Panda