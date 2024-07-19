#pragma once

#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Panda {

struct ProjectSettings final : public Rain::Codable {
    std::string worldPath;

    RAIN_FIELDS_BEGIN(ProjectSettings)
    RAIN_FIELD(worldPath)
    RAIN_FIELDS_END

    void clear() {
        worldPath.clear();
    }
};

} // namespace Panda