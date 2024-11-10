#pragma once

#include "Panda/GameLogic/Components/BaseComponents.hpp"

#include <Rain/Rain.hpp>

namespace Panda {

struct TransformComponentDto : public Rain::Codable {
    Vec3 position;
    Quat rotation;
    Vec3 scale;

    RAIN_FIELDS_BEGIN(TransformComponentDto)
    RAIN_FIELD(position)
    RAIN_FIELD(rotation)
    RAIN_FIELD(scale)
    RAIN_FIELDS_END
};

struct WorldCameraDto : public Rain::Codable {
    std::string projectionType;
    float fieldOfView;
    float orthoSize;
    float zNear;
    float zFar;

    void setProjectionType(WorldCamera::ProjectionType _projectionType) {
        switch (_projectionType) {
            case WorldCamera::ProjectionType::PERSPECTIVE:
                projectionType = "PERSPECTIVE";
            case WorldCamera::ProjectionType::ORTHOGRAPHIC:
                projectionType = "ORTHOGRAPHIC";
        }
        projectionType = "PERSPECTIVE";
    }

    WorldCamera::ProjectionType getProjectionType() {
        if (projectionType == "ORTHOGRAPHIC") {
            return WorldCamera::ProjectionType::ORTHOGRAPHIC;
        }
        return WorldCamera::ProjectionType::PERSPECTIVE;
    }

    RAIN_FIELDS_BEGIN(WorldCameraDto)
    RAIN_FIELD(projectionType)
    RAIN_FIELD(fieldOfView)
    RAIN_FIELD(orthoSize)
    RAIN_FIELD(zNear)
    RAIN_FIELD(zFar)
    RAIN_FIELDS_END
};

struct SpriteRendererComponentDto : public Rain::Codable {
    Color color;
    UUID texture = 0;

    SpriteRendererComponentDto() = default;
    SpriteRendererComponentDto(const SpriteRendererComponentDto &other) = default;

    RAIN_FIELDS_BEGIN(SpriteRendererComponentDto)
    RAIN_FIELD(color)
    RAIN_FIELD(texture)
    RAIN_FIELDS_END
};

struct CameraComponentDto : public Rain::Codable {
    bool isPrimary;
    WorldCameraDto camera;

    RAIN_FIELDS_BEGIN(CameraComponentDto)
    RAIN_FIELD(isPrimary)
    RAIN_FIELD(camera)
    RAIN_FIELDS_END
};

struct CubeMapDto : public Rain::Codable {
    RAIN_FIELDS_BEGIN(CubeMapDto)
    RAIN_FIELDS_END
};

struct ExternalScriptDto : public Rain::Codable {
    std::string name;

    ExternalScriptDto()
        : name() {}

    ExternalScriptDto(const std::string &name)
        : name(name) {}

    RAIN_FIELDS_BEGIN(ExternalScriptDto)
    RAIN_FIELD(name)
    RAIN_FIELDS_END
};

struct ScriptListComponentDto : public Rain::Codable {
    std::vector<ExternalScriptDto> scripts;

    RAIN_FIELDS_BEGIN(ScriptListComponentDto)
    RAIN_FIELD(scripts)
    RAIN_FIELDS_END
};

} // namespace Panda