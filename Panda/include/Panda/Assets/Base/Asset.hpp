#pragma once

#include <Foundation/Foundation.hpp>
#include <Rain/UUID.hpp>
#include <cstdint>
#include <filesystem>

namespace Panda {

enum class AssetType { NONE = 0, TEXTURE, CUBE_MAP, SHADER, MATERIAL };

using AssetId = UUID;

/// Asset отвечает за хранение в памяти и некоторую работу ассета.
class Asset {
public:
    Asset(AssetType type)
        : m_type(type) {}
    Asset()
        : m_type(AssetType::NONE) {}

    virtual ~Asset() = default;

    AssetType getType() {
        return m_type;
    }

protected:
    AssetType m_type;
};

} // namespace Panda