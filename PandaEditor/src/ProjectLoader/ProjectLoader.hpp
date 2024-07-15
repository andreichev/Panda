#pragma once

#include "Panda/GameLogic/World.hpp"

#include <filesystem>

namespace Panda {

class ProjectLoader final {
public:
    ProjectLoader();
    void openProject();
    std::optional<World> openWorld(const std::filesystem::path &path);

    bool hasOpenedWorld();
    bool hasOpenedProject();

private:
    std::filesystem::path m_projectPath;
    std::filesystem::path m_worldPath;
};

} // namespace Panda