#pragma once

#include "AssetRegistryEditorDto.hpp"
#include "Model/AssetInfoEditor.hpp"

namespace Panda {

class AssetRegistryMapper final {
public:
    static AssetInfoEditorDto toDto(const AssetInfoEditor &info) {
        AssetInfoEditorDto dto;
        dto.id = info.id;
        dto.path = info.path.string();
        dto.setAssetType(info.type);
        return dto;
    }

    static AssetInfoEditor toInfo(const AssetInfoEditorDto &dto) {
        AssetInfoEditor info;
        info.id = dto.id;
        info.path = dto.path;
        info.type = dto.getAssetType();
        return info;
    }
};

} // namespace Panda