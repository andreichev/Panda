#pragma once

#include "Panda/Base/Base.hpp"

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>
#include <variant>

namespace Panda {

// TODO: Add Vec2, Vec3, Mat3, Mat4
enum class MaterialFieldType : uint32_t { FLOAT, VEC4, TEXTURE, UNKNOWN };

using MaterialFieldDtoData = std::variant<int, float, Vec4, UUID>;

struct MaterialFieldDto : public Rain::Codable {
    std::string name;
    MaterialFieldType type;
    MaterialFieldDtoData value;

    static void encode(const char *key, Rain::Encoder *encoder, const MaterialFieldDto &data) {
        encoder->beginObject(key);
        encoder->encode("name", data.name);
        encoder->encode("type", data.type);
        switch (data.type) {
            case MaterialFieldType::FLOAT: {
                float value = std::get<float>(data.value);
                encoder->encode("value", value);
                break;
            }
            case MaterialFieldType::VEC4: {
                Vec4 value = std::get<Vec4>(data.value);
                encoder->encode("value", value);
                break;
            }
            case MaterialFieldType::TEXTURE: {
                UUID value = std::get<UUID>(data.value);
                encoder->encode("value", value);
                break;
            }
            default: {
                PND_ASSERT(false, "Unknown field type");
                break;
            }
        }
        encoder->endObject();
    }

    static bool decode(const char *key, Rain::Decoder *decoder, MaterialFieldDto &data) {
        if (!decoder->beginObject(key)) { return false; }
        decoder->decode("name", data.name);
        decoder->decode("type", data.type);
        switch (data.type) {
            case MaterialFieldType::FLOAT: {
                float value = 0;
                decoder->decode("value", value);
                data.value = value;
                break;
            }
            case MaterialFieldType::VEC4: {
                Vec4 value;
                decoder->decode("value", value);
                data.value = value;
                break;
            }
            case MaterialFieldType::TEXTURE: {
                UUID value;
                decoder->decode("value", value);
                data.value = value;
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

struct MaterialDataDto : public Rain::Codable {
    std::vector<MaterialFieldDto> inputs;

    MaterialDataDto()
        : inputs() {}

    RAIN_FIELDS_BEGIN(MaterialDataDto)
    RAIN_FIELD(inputs)
    RAIN_FIELDS_END
};

} // namespace Panda