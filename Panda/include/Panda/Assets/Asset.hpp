#pragma once

#include <Foundation/Foundation.hpp>
#include <Rain/UUID.hpp>
#include <cstdint>
#include <filesystem>

namespace Panda {

enum class AssetType : uint16_t {
    NONE = 0,
    TEXTURE,
    CUBE_MAP,
    PROGRAM,
};

using AssetId = UUID;

class Asset {
public:
    virtual ~Asset() = default;

    virtual bool operator==(const Asset &other) const {
        return m_handle == other.m_handle;
    }

    virtual bool operator!=(const Asset &other) const {
        return !(*this == other);
    }

private:
    AssetId m_handle = 0;
    AssetType m_type;
};

} // namespace Panda