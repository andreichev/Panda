#pragma once

#include <Foundation/Foundation.hpp>
#include <Rain/UUID.hpp>
#include <cstdint>
#include <filesystem>

namespace Panda {

enum class AssetType {
    NONE = 0,
    TEXTURE,
    CUBE_MAP,
    PROGRAM,
};

using AssetId = UUID;

/// Asset отвечает за хранение в памяти и некоторую работу ассета.
class Asset {
public:
    virtual ~Asset() = default;

    virtual bool operator==(const Asset &other) const {
        return m_handle == other.m_handle;
    }

    virtual bool operator!=(const Asset &other) const {
        return !(*this == other);
    }

protected:
    AssetId m_handle = 0;
    AssetType m_type = AssetType::NONE;
};

} // namespace Panda