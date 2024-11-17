#pragma once

#include <Panda/Assets/Asset.hpp>
#include <Rain/Rain.hpp>

namespace Panda {

struct AssetInfoEditorDto : public Rain::Codable {
    AssetId id;
    std::string type;
    std::string path;

    AssetType getAssetType() const {
        if (type == "TEXTURE") {
            return AssetType::TEXTURE;
        } else if (type == "CUBE_MAP") {
            return AssetType::CUBE_MAP;
        } else if (type == "PROGRAM") {
            return AssetType::PROGRAM;
        }
        return AssetType::NONE;
    }

    void setAssetType(AssetType assetType) {
        switch (assetType) {
            case AssetType::TEXTURE: {
                type = "TEXTURE";
                break;
            }
            case AssetType::CUBE_MAP: {
                type = "CUBE_MAP";
                break;
            }
            case AssetType::PROGRAM: {
                type = "PROGRAM";
                break;
            }
            case AssetType::NONE: {
                type = "NONE";
                break;
            }
        }
    }

    RAIN_FIELDS_BEGIN(AssetInfoEditorDto)
    RAIN_FIELD(id)
    RAIN_FIELD(type)
    RAIN_FIELD(path)
    RAIN_FIELDS_END
};

struct AssetRegistryDto : public Rain::Codable {
    std::vector<AssetInfoEditorDto> assets;

    RAIN_FIELDS_BEGIN(AssetRegistryDto)
    RAIN_FIELD(assets)
    RAIN_FIELDS_END
};

} // namespace Panda