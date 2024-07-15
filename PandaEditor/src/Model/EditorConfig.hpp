#pragma once

#include "RecentProject.hpp"

namespace Panda {

struct EditorConfig final : Rain::Codable {
    std::vector<RecentProject> recentProjects;

    RAIN_FIELDS_BEGIN(EditorConfig)
    RAIN_FIELD(recentProjects)
    RAIN_FIELDS_END
};

} // namespace Panda