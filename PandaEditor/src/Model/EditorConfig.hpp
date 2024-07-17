#pragma once

#include "RecentProject.hpp"

namespace Panda {

struct EditorConfig final : public Rain::Codable {
    std::vector<RecentProject> recentProjects;
    bool hasOpenedProject;

    RAIN_FIELDS_BEGIN(EditorConfig)
    RAIN_FIELD(recentProjects)
    RAIN_FIELD(hasOpenedProject)
    RAIN_FIELDS_END
};

} // namespace Panda