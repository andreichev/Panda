#pragma once

#include "Panda/GameLogic/World.hpp"
#include "Model/EditorConfig.hpp"

#include <Rain/Coders/JsonEncoder.hpp>
#include <Rain/Coders/JsonDecoder.hpp>
#include <filesystem>

namespace Panda {

class ProjectLoader final {
public:
    ProjectLoader();
    ~ProjectLoader();
    void openProject(const std::filesystem::path &path);
    void createProject(const std::filesystem::path &path);
    std::optional<World> openWorld(const std::filesystem::path &path);
    const std::vector<RecentProject> &getRecentProjectsList();
    bool hasOpenedWorld();

    bool hasOpenedProject();

private:
    void appendRecentProject();

    Rain::JsonEncoder jsonEncoder;
    Rain::JsonDecoder jsonDecoder;
    EditorConfig m_config;
    std::filesystem::path m_projectPath;
    std::filesystem::path m_worldPath;
};

} // namespace Panda