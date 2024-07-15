#pragma once

#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Panda {

struct RecentProject final : Rain::Codable {
    std::string path;
    std::string name;

    RAIN_FIELDS_BEGIN(RecentProject)
    RAIN_FIELD(path)
    RAIN_FIELD(name)
    RAIN_FIELDS_END
};

} // namespace Panda