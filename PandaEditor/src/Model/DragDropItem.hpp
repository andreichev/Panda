#pragma once

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>

namespace Panda {

static const char *PANDA_DRAGDROP_NAME = "PANDA_DRAGDROP";

enum class DragDropItemType { TEXTURE, ENTITY };

struct DragDropItem {
    DragDropItemType type;
    UUID assetId;
    Foundation::Memory data;
};

} // namespace Panda