#pragma once

#include <Rain/Rain.hpp>

namespace Panda {

struct RecentProject final : public Rain::Codable {
    std::string path;
    std::string name;

    RAIN_FIELDS_BEGIN(RecentProject)
    RAIN_FIELD(path)
    RAIN_FIELD(name)
    RAIN_FIELDS_END

    bool operator==(const RecentProject &other) const {
        return path == other.path;
    }
};

} // namespace Panda