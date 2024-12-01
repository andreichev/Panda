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
            case WorldCamera::ProjectionType::PERSPECTIVE: {
                projectionType = "PERSPECTIVE";
                return;
            }
            case WorldCamera::ProjectionType::ORTHOGRAPHIC: {
                projectionType = "ORTHOGRAPHIC";
                return;
            }
        }
        PND_ASSERT(false, "Unknown projection type");
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

struct Rigidbody2DComponentDto : public Rain::Codable {
    std::string type;
    bool fixedRotation;

    void setType(Rigidbody2DComponent::BodyType _type) {
        switch (_type) {
            case Rigidbody2DComponent::BodyType::STATIC: {
                type = "STATIC";
                return;
            }
            case Rigidbody2DComponent::BodyType::DYNAMIC: {
                type = "DYNAMIC";
                return;
            }
            case Rigidbody2DComponent::BodyType::KINEMATIC: {
                type = "KINEMATIC";
                return;
            }
        }
        PND_ASSERT(false, "Unknown rigidbody body type");
        type = "STATIC";
    }

    Rigidbody2DComponent::BodyType getType() {
        if (type == "STATIC") {
            return Rigidbody2DComponent::BodyType::STATIC;
        }
        if (type == "DYNAMIC") {
            return Rigidbody2DComponent::BodyType::DYNAMIC;
        }
        if (type == "KINEMATIC") {
            return Rigidbody2DComponent::BodyType::KINEMATIC;
        }
        PND_ASSERT(false, "Unknown rigidbody body type");
        return Rigidbody2DComponent::BodyType::STATIC;
    }

    RAIN_FIELDS_BEGIN(Rigidbody2DComponentDto)
    RAIN_FIELD(type)
    RAIN_FIELD(fixedRotation)
    RAIN_FIELDS_END
};

struct BoxCollider2DComponentDto : public Rain::Codable {
    Vec2 offset;
    Vec2 size;
    float density;
    float friction;
    float restitution;

    RAIN_FIELDS_BEGIN(BoxCollider2DComponentDto)
    RAIN_FIELD(offset)
    RAIN_FIELD(size)
    RAIN_FIELD(density)
    RAIN_FIELD(friction)
    RAIN_FIELD(restitution)
    RAIN_FIELDS_END
};

struct ScriptFieldDto : public Rain::Codable {
    std::string name;
    int type;

    void setType(ScriptFieldType _type) {
        type = (int)_type;
    }

    ScriptFieldType getType() {
        return (ScriptFieldType)type;
    }

    RAIN_FIELDS_BEGIN(ScriptFieldDto)
    RAIN_FIELD(name)
    RAIN_FIELD(type)
    RAIN_FIELDS_END
};

struct ExternalScriptDto : public Rain::Codable {
    std::string name;
    std::vector<ScriptFieldDto> scriptFields;

    ExternalScriptDto()
        : name()
        , scriptFields() {}

    ExternalScriptDto(const std::string &name, const std::vector<ScriptFieldDto> &fields)
        : name(name)
        , scriptFields(fields) {}

    RAIN_FIELDS_BEGIN(ExternalScriptDto)
    RAIN_FIELD(name)
    RAIN_FIELD(scriptFields)
    RAIN_FIELDS_END
};

struct ScriptListComponentDto : public Rain::Codable {
    std::vector<ExternalScriptDto> scripts;

    RAIN_FIELDS_BEGIN(ScriptListComponentDto)
    RAIN_FIELD(scripts)
    RAIN_FIELDS_END
};

} // namespace Panda