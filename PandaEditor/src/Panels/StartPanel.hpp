#pragma once

#include "Model/RecentProject.hpp"
#include "Panda/ImGui/FontAwesome.h"
#include "Panda/ImGui/ImGuiFonts.hpp"
#include "Panda/Application/Application.hpp"
#include "ProjectLoader/ProjectLoader.hpp"

namespace Panda {

class StartPanel final {
public:
    StartPanel(ProjectLoader *loader);
    void onImGuiRender();

private:
    void createProject(const std::string &name);
    void openProject();

    Texture m_logoImage;
    Texture m_iconProject;
    bool m_newProjectMenu;

    ProjectLoader *m_loader;
};

} // namespace Panda