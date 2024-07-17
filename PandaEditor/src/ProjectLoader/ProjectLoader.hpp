#pragma once

#include "Panda/GameLogic/World.hpp"
#include "Model/EditorConfig.hpp"
#include "FileSystem/FileSystem.hpp"

#include <Rain/Coders/JsonEncoder.hpp>
#include <Rain/Coders/JsonDecoder.hpp>

namespace Panda {

class ProjectLoader final {
public:
    ProjectLoader();
    ~ProjectLoader();
    void loadData();
    void saveAppConfig();
    void openProject(const path_t &path);
    void createProject(const path_t &path);
    const std::vector<RecentProject> &getRecentProjectsList();
    bool hasOpenedProject();
    void saveWorld(World *world);
    void saveWorldAs(World *world);

private:
    void loadRecentProject();
    void appendRecentProject();

    Rain::JsonEncoder jsonEncoder;
    Rain::JsonDecoder jsonDecoder;
    EditorConfig m_config;
    path_t m_projectPath;
    path_t m_worldPath;
};

} // namespace Panda