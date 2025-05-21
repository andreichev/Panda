//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include "Serialization/RecentProject.hpp"
#include "SystemTools/SystemTools.hpp"
#include "Panda/Assets/TextureAsset.hpp"

#include <imgui.h>

namespace Panda {

class MenuBarOutput {
public:
    virtual ~MenuBarOutput() = default;
    virtual void menuBarOpenProject() = 0;
    virtual void menuBarOpenProject(const RecentProject &project) = 0;
    virtual void menuBarSaveWorld() = 0;
    virtual void menuBarOpenCppProject() = 0;
    virtual void menuBarCloseProject() = 0;
    virtual void menuBarCloseApp() = 0;
    virtual bool menuBarCanUndo() = 0;
    virtual void menuBarUndo() = 0;
    virtual bool menuBarCanRedo() = 0;
    virtual void menuBarRedo() = 0;
    virtual void menuBarAbout() = 0;
    virtual const path_t &menuBarGetOpenedProjectPath() = 0;
    virtual const std::vector<RecentProject> &menuBarGetRecentProjectsList() = 0;
};

class MenuBar final {
public:
    MenuBar(MenuBarOutput *output);
    void onImGuiRender();

    float m_height;
    MenuBarOutput *m_output;
};

} // namespace Panda