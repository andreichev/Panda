#pragma once

#include "Panda/GameLogic/World.hpp"
#include "Model/EditorSettings.hpp"
#include "FileSystem/FileSystem.hpp"
#include "Model/ProjectSettings.hpp"

#include <Rain/Coders/JsonEncoder.hpp>
#include <Rain/Coders/JsonDecoder.hpp>

namespace Panda {

class ProjectLoaderOutput {
public:
    virtual ~ProjectLoaderOutput() = default;
    virtual void loaderDidLoadProject() = 0;
    virtual void loaderDidLoadCloseProject() = 0;
    virtual void loaderDidLoadWorld(World &&world) = 0;
};

class ProjectLoader final {
public:
    ProjectLoader(ProjectLoaderOutput *output);
    ~ProjectLoader();
    void loadInitialData();
    void saveAppSettings();
    void saveProjectSettings();
    void openProject(const path_t &path);
    void loadWorld();
    void closeProject();
    void createProject(const path_t &path);
    const std::vector<RecentProject> &getRecentProjectsList();
    bool hasOpenedProject();
    void saveWorld(const World &world);
    void removeRecentProject(int index);
    const ProjectSettings &getProjectSettings();

private:
    void saveWorldAs(const World &world);
    void loadRecentProject();
    void appendRecentProject();

    Rain::JsonEncoder jsonEncoder;
    Rain::JsonDecoder jsonDecoder;
    EditorSettings m_editorSettings;
    ProjectSettings m_projectSettings;
    path_t m_projectPath;
    path_t m_worldPath;
    ProjectLoaderOutput *m_output;
};

} // namespace Panda