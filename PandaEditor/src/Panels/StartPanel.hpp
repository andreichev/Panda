#pragma once

#include "Model/RecentProject.hpp"
#include "Panda/ImGui/FontAwesome.h"
#include "Panda/ImGui/ImGuiFonts.hpp"
#include "Panda/Application/Application.hpp"
#include "ProjectLoader/ProjectLoader.hpp"

namespace Panda {

class StartPanel final {
public:
    StartPanel();

    void onImGuiRender();
    void setRecentProjects(std::vector<RecentProject> recentProjects) {
        m_recentProjects = recentProjects;
    }

private:
    void newProject();
    void openProject();
    std::optional<std::string> openFolder();

    Texture m_logoImage;
    Texture m_iconProject;
    bool m_newProjectMenu;

    std::vector<RecentProject> m_recentProjects;
};

} // namespace Panda