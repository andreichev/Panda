#include "ProjectLoader.hpp"

namespace Panda {

ProjectLoader::ProjectLoader() {}

void ProjectLoader::openProject() {}

std::optional<World> ProjectLoader::openWorld(const std::filesystem::path &path) {
    return {};
}

bool ProjectLoader::hasOpenedWorld() {
    return false;
}

bool ProjectLoader::hasOpenedProject() {
    return false;
}

} // namespace Panda