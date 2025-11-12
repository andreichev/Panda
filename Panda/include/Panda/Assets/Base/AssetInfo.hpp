#pragma once

#include "Panda/Assets/Base/Path.hpp"
#include "Panda/Assets/Base/Asset.hpp"
#include "Panda/Serialization/AssetsDtos.hpp"

#include <Rain/Rain.hpp>
#include <Miren/Miren.hpp>
#include <variant>

namespace Panda {

struct TextureAssetMeta : public Rain::Codable {
    Miren::TextureFiltering minFiltering = Miren::NEAREST;
    Miren::TextureFiltering magFiltering = Miren::NEAREST;
    uint32_t numMips = 0;
    path_t path;

    RAIN_FIELDS_BEGIN(TextureAssetMeta)
    RAIN_FIELD(minFiltering)
    RAIN_FIELD(magFiltering)
    RAIN_FIELD(numMips)
    RAIN_FIELD(path)
    RAIN_FIELDS_END
};

struct ShaderAssetMeta : public Rain::Codable {
    path_t vertexCodePath;
    path_t fragmentCodePath;

    RAIN_FIELDS_BEGIN(ShaderAssetMeta)
    RAIN_FIELD(vertexCodePath)
    RAIN_FIELD(fragmentCodePath)
    RAIN_FIELDS_END
};

struct MaterialAssetMeta : public Rain::Codable {
    path_t materialPath;
    UUID shader;

    RAIN_FIELDS_BEGIN(MaterialAssetMeta)
    RAIN_FIELD(materialPath)
    RAIN_FIELD(shader)
    RAIN_FIELDS_END
};

using AssetMeta = std::variant<TextureAssetMeta, ShaderAssetMeta, MaterialAssetMeta>;

struct AssetInfo : public Rain::Codable {
    AssetId id;
    AssetType type;
    AssetMeta meta;

    static void encode(const char *key, Rain::Encoder *encoder, const AssetInfo &data) {
        encoder->beginObject(key);
        encoder->encode("id", data.id);
        encoder->encode("type", data.type);
        switch (data.type) {
            case AssetType::TEXTURE: {
                TextureAssetMeta meta = std::get<TextureAssetMeta>(data.meta);
                encoder->encode("meta", meta);
                break;
            }
            case AssetType::SHADER: {
                ShaderAssetMeta meta = std::get<ShaderAssetMeta>(data.meta);
                encoder->encode("meta", meta);
                break;
            }
            case AssetType::MATERIAL: {
                MaterialAssetMeta meta = std::get<MaterialAssetMeta>(data.meta);
                encoder->encode("meta", meta);
                break;
            }
            default: {
                break;
            }
        }
        encoder->endObject();
    }

    static bool decode(const char *key, Rain::Decoder *decoder, AssetInfo &data) {
        if (!decoder->beginObject(key)) { return false; }
        decoder->decode("id", data.id);
        decoder->decode("type", data.type);
        switch (data.type) {
            case AssetType::TEXTURE: {
                TextureAssetMeta meta;
                decoder->decode("meta", meta);
                data.meta = meta;
                break;
            }
            case AssetType::SHADER: {
                ShaderAssetMeta meta;
                decoder->decode("meta", meta);
                data.meta = meta;
                break;
            }
            case AssetType::MATERIAL: {
                MaterialAssetMeta meta;
                decoder->decode("meta", meta);
                data.meta = meta;
                break;
            }
            default: {
                break;
            }
        }
        decoder->endObject();
        return true;
    }

    const char *getTypeStr() {
        switch (type) {
            case AssetType::TEXTURE: {
                return "Texture";
            }
            case AssetType::SHADER: {
                return "Shader";
            }
            case AssetType::MATERIAL: {
                return "Material";
            }
            default: {
                return "Unknown";
            }
        }
    }
};

struct AssetRegistryDto : public Rain::Codable {
    std::vector<AssetInfo> assets;

    RAIN_FIELDS_BEGIN(AssetRegistryDto)
    RAIN_FIELD(assets)
    RAIN_FIELDS_END
};

} // namespace Panda