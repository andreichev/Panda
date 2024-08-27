#pragma once

#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Panda {

struct EditorCameraSettings : public Rain::Codable {
    Vec3 position;
    Quat rotation;

    RAIN_FIELDS_BEGIN(EditorCameraSettings)
    RAIN_FIELD(position)
    RAIN_FIELD(rotation)
    RAIN_FIELDS_END

    void reset() {
        position = {0, 0, 0};
        rotation = {0, 0, 0, 0};
    }
};

struct ProjectSettings final : public Rain::Codable {
    std::string worldPath;
    EditorCameraSettings editorCameraSettings;

    RAIN_FIELDS_BEGIN(ProjectSettings)
    RAIN_FIELD(worldPath)
    RAIN_FIELD(editorCameraSettings)
    RAIN_FIELDS_END

    void clear() {
        worldPath.clear();
        editorCameraSettings.reset();
    }
};

} // namespace Panda