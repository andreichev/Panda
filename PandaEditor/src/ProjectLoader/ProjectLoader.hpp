#pragma once

#include "AssetHandlerEditor.hpp"
#include "Panda/GameLogic/World.hpp"
#include "Panda/ScriptEngine/ScriptEngine.hpp"
#include "Serialization/EditorSettings.hpp"
#include "SystemTools/ProjectCreator.hpp"
#include "Serialization/ProjectSettings.hpp"

#include <Rain/Coders/JsonEncoder.hpp>
#include <Rain/Coders/JsonDecoder.hpp>

namespace Panda {

class ProjectLoaderOutput {
public:
    virtual ~ProjectLoaderOutput() = default;
    virtual void loaderDidLoadProject(const std::string &name, const path_t &path) = 0;
    virtual void loaderDidLoadCloseProject() = 0;
    virtual void loaderDidLoadWorld() = 0;
    virtual void loaderCreateSampleWorld() = 0;
    virtual EditorCameraSettings getEditorCameraSettings() = 0;
    virtual void setEditorCameraSettings(EditorCameraSettings settings) = 0;
};

class ProjectLoader final {
public:
    ProjectLoader(World *world, ProjectLoaderOutput *output);
    ~ProjectLoader();
    void loadInitialData();
    void saveAppSettings();
    void saveWindowState(LastOpenedProjectWindowState state);
    void saveProjectSettings();
    void openProject(const path_t &path);
    void openCppProject();
    void showCppProject();
    void reloadScriptsDll();
    void loadWorld();
    void saveWorld();
    void closeProject();
    void createProject(const std::string &name, const path_t &path);
    const std::vector<RecentProject> &getRecentProjectsList();
    bool hasOpenedProject();
    const path_t &getOpenedProjectPath();
    void removeRecentProject(int index);
    const ProjectSettings &getProjectSettings();
    LastOpenedProjectWindowState getLastWindowState();

private:
    void saveWorldAs();
    void loadRecentProject();
    void appendRecentProject();

    Rain::JsonEncoder m_jsonEncoder;
    Rain::JsonDecoder m_jsonDecoder;
    EditorSettings m_editorSettings;
    ProjectSettings m_projectSettings;
    ProjectCreator m_projectCreator;
    path_t m_projectPath;
    path_t m_worldPath;
    World *m_world;
    ProjectLoaderOutput *m_output;
    ScriptEngine m_scriptEngine;
    AssetHandlerEditor *m_assetHandler;
};

} // namespace Panda