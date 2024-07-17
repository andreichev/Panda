#pragma once

#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Panda {

struct ProjectConfig final : public Rain::Codable {
    std::vector<std::string> worlds;

    RAIN_FIELDS_BEGIN(ProjectConfig)
    RAIN_FIELD(worlds)
    RAIN_FIELDS_END
};

} // namespace Panda