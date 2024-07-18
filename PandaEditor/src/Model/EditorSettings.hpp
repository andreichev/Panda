#pragma once

#include "RecentProject.hpp"

namespace Panda {

struct EditorSettings final : public Rain::Codable {
    std::vector<RecentProject> recentProjects;
    bool hasOpenedProject;

    RAIN_FIELDS_BEGIN(EditorSettings)
    RAIN_FIELD(recentProjects)
    RAIN_FIELD(hasOpenedProject)
    RAIN_FIELDS_END
};

} // namespace Panda