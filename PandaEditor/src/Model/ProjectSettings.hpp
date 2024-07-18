#pragma once

#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Panda {

struct ProjectSettings final : public Rain::Codable {
    std::string lastWorld;

    RAIN_FIELDS_BEGIN(ProjectSettings)
    RAIN_FIELD(lastWorld)
    RAIN_FIELDS_END
};

} // namespace Panda