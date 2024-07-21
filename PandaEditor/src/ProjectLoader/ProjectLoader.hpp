#pragma once

#include "Panda/GameLogic/World.hpp"
#include "Model/EditorSettings.hpp"
#include "SystemTools/SystemTools.hpp"
#include "Model/ProjectSettings.hpp"

#include <Rain/Coders/JsonEncoder.hpp>
#include <Rain/Coders/JsonDecoder.hpp>

namespace Panda {

class ProjectLoaderOutput {
public:
    virtual ~ProjectLoaderOutput() = default;
    virtual void loaderDidLoadProject(const path_t &path) = 0;
    virtual void loaderDidLoadCloseProject() = 0;
    virtual void loaderDidLoadWorld() = 0;
    virtual void loaderCreateSampleWorld() = 0;
};

class ProjectLoader final {
public:
    ProjectLoader(World *world, ProjectLoaderOutput *output);
    ~ProjectLoader();
    void loadInitialData();
    void saveAppSettings();
    void saveProjectSettings();
    void openProject(const path_t &path);
    void loadWorld();
    void saveWorld();
    void closeProject();
    void createProject(const path_t &path);
    const std::vector<RecentProject> &getRecentProjectsList();
    bool hasOpenedProject();
    void removeRecentProject(int index);
    const ProjectSettings &getProjectSettings();

private:
    void saveWorldAs();
    void loadRecentProject();
    void appendRecentProject();

    Rain::JsonEncoder jsonEncoder;
    Rain::JsonDecoder jsonDecoder;
    EditorSettings m_editorSettings;
    ProjectSettings m_projectSettings;
    path_t m_projectPath;
    path_t m_worldPath;
    World *m_world;
    ProjectLoaderOutput *m_output;
};

} // namespace Panda