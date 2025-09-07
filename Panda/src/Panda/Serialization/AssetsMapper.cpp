#include "Panda/Serialization/AssetsMapper.hpp"
#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/Base/Base.hpp"

namespace Panda {

void AssetsMapper::toData(MaterialData &data, const MaterialDataDto &dto) {
    for (auto &fieldDto : dto.inputs) {
        MaterialField field;
        field.name = fieldDto.name;
        field.type = fieldDto.type;
        switch (fieldDto.type) {
            case MaterialFieldType::INTEGER: {
                field.value = std::get<int>(fieldDto.value);
                break;
            }
            case MaterialFieldType::FLOAT: {
                field.value = std::get<float>(fieldDto.value);
                break;
            }
            case MaterialFieldType::VEC4: {
                field.value = std::get<Vec4>(fieldDto.value);
                break;
            }
            case MaterialFieldType::TEXTURE: {
                UUID assetId = std::get<UUID>(fieldDto.value);
                AssetHandler *handler = GameContext::getAssetHandler();
                PND_ASSERT(handler != nullptr, "INVALID ASSET HANDLER");
                auto texture = handler->makeRef<Asset>(assetId);
                field.value = texture;
                break;
            }
            default: {
                PND_ASSERT(false, "Unknown field type");
                break;
            }
        }
        data.inputs.emplace_back(field);
    }
}

void AssetsMapper::toDto(const MaterialData &data, MaterialDataDto &dto) {
    for (auto &field : data.inputs) {
        MaterialFieldDto fieldDto;
        fieldDto.name = field.name;
        fieldDto.type = field.type;
        switch (field.type) {
            case MaterialFieldType::INTEGER: {
                fieldDto.value = std::get<int>(field.value);
                break;
            }
            case MaterialFieldType::FLOAT: {
                fieldDto.value = std::get<float>(field.value);
                break;
            }
            case MaterialFieldType::VEC4: {
                fieldDto.value = std::get<Vec4>(field.value);
                break;
            }
            case MaterialFieldType::TEXTURE: {
                auto assetRef = std::get<AssetRef<Asset>>(field.value);
                fieldDto.value = assetRef.getId();
                break;
            }
            default: {
                PND_ASSERT(false, "Unknown field type");
                break;
            }
        }
        dto.inputs.emplace_back(fieldDto);
    }
}

} // namespace Panda