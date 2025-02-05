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
    WorldCamera::ProjectionType projectionType;
    float fieldOfView;
    float orthoSize;
    float zNear;
    float zFar;

    static void encode(const char *key, Rain::Encoder *encoder, WorldCameraDto &data) {
        encoder->beginObject(key);
        int projectionType = (int)data.projectionType;
        encoder->encode("projectionType", projectionType);
        encoder->encode("fieldOfView", data.fieldOfView);
        encoder->encode("orthoSize", data.orthoSize);
        encoder->encode("zNear", data.zNear);
        encoder->encode("zFar", data.zFar);
        encoder->endObject();
    }

    static bool decode(const char *key, Rain::Decoder *decoder, WorldCameraDto &data) {
        if (!decoder->beginObject(key)) { return false; }
        int projectionType;
        decoder->decode("projectionType", projectionType);
        data.projectionType = (WorldCamera::ProjectionType)projectionType;
        decoder->decode("fieldOfView", data.fieldOfView);
        decoder->decode("orthoSize", data.orthoSize);
        decoder->decode("zNear", data.zNear);
        decoder->decode("zFar", data.zFar);
        decoder->endObject();
        return true;
    }
};

struct SpriteRendererComponentDto : public Rain::Codable {
    Color color;
    int cols = 1;
    int rows = 1;
    int index = 0;
    UUID texture = 0;

    SpriteRendererComponentDto() = default;
    SpriteRendererComponentDto(const SpriteRendererComponentDto &other) = default;

    RAIN_FIELDS_BEGIN(SpriteRendererComponentDto)
    RAIN_FIELD(color)
    RAIN_FIELD(cols)
    RAIN_FIELD(rows)
    RAIN_FIELD(index)
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

struct CubeMapDto : public Rain::Codable {};

struct Rigidbody2DComponentDto : public Rain::Codable {
    Rigidbody2DComponent::BodyType type;
    bool fixedRotation;

    static void encode(const char *key, Rain::Encoder *encoder, Rigidbody2DComponentDto &data) {
        encoder->beginObject(key);
        int type = (int)data.type;
        encoder->encode("type", type);
        encoder->encode("fixedRotation", data.fixedRotation);
        encoder->endObject();
    }

    static bool decode(const char *key, Rain::Decoder *decoder, Rigidbody2DComponentDto &data) {
        if (!decoder->beginObject(key)) { return false; }
        int type;
        decoder->decode("type", type);
        data.type = (Rigidbody2DComponent::BodyType)type;
        decoder->decode("fixedRotation", data.fixedRotation);
        decoder->endObject();
        return true;
    }
};

struct BoxCollider2DComponentDto : public Rain::Codable {
    bool isSensor;
    Vec2 offset;
    Vec2 size;
    float density;
    float friction;
    float restitution;

    RAIN_FIELDS_BEGIN(BoxCollider2DComponentDto)
    RAIN_FIELD(isSensor)
    RAIN_FIELD(offset)
    RAIN_FIELD(size)
    RAIN_FIELD(density)
    RAIN_FIELD(friction)
    RAIN_FIELD(restitution)
    RAIN_FIELDS_END
};

struct ScriptFieldDto : public Rain::Codable {
    std::string name;
    uint32_t fieldId;
    ScriptFieldType type;
    Foundation::Memory value;

    static void encode(const char *key, Rain::Encoder *encoder, ScriptFieldDto &data) {
        encoder->beginObject(key);
        encoder->encode("name", data.name);
        encoder->encode("fieldId", data.fieldId);
        int type = (int)data.type;
        encoder->encode("type", type);
        switch (data.type) {
            case ScriptFieldType::INTEGER: {
                int *value = (int *)data.value.data;
                encoder->encode("value", *value);
                break;
            }
            case ScriptFieldType::FLOAT: {
                float *value = (float *)data.value.data;
                encoder->encode("value", *value);
                break;
            }
            case ScriptFieldType::ENTITY:
            case ScriptFieldType::TEXTURE: {
                UUID *value = (UUID *)data.value.data;
                encoder->encode("value", *value);
                break;
            }
            default: {
                PND_ASSERT(false, "Unknown field type");
                break;
            }
        }
        encoder->endObject();
    }

    static bool decode(const char *key, Rain::Decoder *decoder, ScriptFieldDto &data) {
        if (!decoder->beginObject(key)) { return false; }
        decoder->decode("name", data.name);
        decoder->decode("fieldId", data.fieldId);
        int type;
        decoder->decode("type", type);
        data.type = (ScriptFieldType)type;
        data.value.release();
        switch (data.type) {
            case ScriptFieldType::INTEGER: {
                data.value = Foundation::Memory::alloc(sizeof(int));
                int value = 0;
                decoder->decode("value", value);
                memcpy(data.value.data, &value, sizeof(int));
                break;
            }
            case ScriptFieldType::FLOAT: {
                data.value = Foundation::Memory::alloc(sizeof(float));
                float value = 0;
                decoder->decode("value", value);
                memcpy(data.value.data, &value, sizeof(float));
                break;
            }
            case ScriptFieldType::TEXTURE:
            case ScriptFieldType::ENTITY: {
                data.value = Foundation::Memory::alloc(sizeof(UUID));
                UUID value = 0;
                decoder->decode("value", value);
                memcpy(data.value.data, &value, sizeof(UUID));
                break;
            }
            default: {
                PND_ASSERT(false, "Unknown field type");
                break;
            }
        }
        decoder->endObject();
        return true;
    }
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