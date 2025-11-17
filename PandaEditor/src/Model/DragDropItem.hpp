#pragma once

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>

namespace Panda {

static const char *PANDA_DRAGDROP_NAME = "PANDA_DRAGDROP";

enum class DragDropItemType { TEXTURE, ENTITY, SHADER, MATERIAL };

struct DragDropItem {
    DragDropItemType type;
    int count;
    // TODO: replace with std::variant
    uint8_t data[1024];
};

} // namespace Panda