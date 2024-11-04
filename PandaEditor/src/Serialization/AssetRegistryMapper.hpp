#pragma once

#include "AssetRegistryEditorDto.hpp"
#include "Model/AssetInfoEditor.hpp"

namespace Panda {

class AssetRegistryMapper final {
public:
    static AssetInfoEditorDto toDto(const AssetInfoEditor &entity) {
        AssetInfoEditorDto dto;
        dto.id = entity.id;
        dto.path = entity.path;
        dto.setAssetType(entity.type);
        return dto;
    }

    static AssetInfoEditor toEntity(const AssetInfoEditorDto &dto) {
        AssetInfoEditor entity;
        entity.id = dto.id;
        entity.path = dto.path;
        entity.type = dto.getAssetType();
        return entity;
    }
};

} // namespace Panda