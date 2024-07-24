#pragma once

#include <Foundation/Foundation.hpp>
#include <cstdint>

namespace Panda {

enum class AssetType : uint16_t { NONE = 0, ENVIRONMENT };

class Asset {
public:
    Foundation::Memory data;
    AssetType m_type;
};

} // namespace Panda