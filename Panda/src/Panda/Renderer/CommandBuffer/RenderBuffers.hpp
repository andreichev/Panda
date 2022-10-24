#pragma once

#include "Panda/Renderer/MirenHandleAllocator.hpp"

namespace Panda {

#define TRANSIENT_VERTEX_BUFFER_SIZE 10000000
#define TRANSIENT_INDEX_BUFFER_SIZE 10000000

struct TransientIndexBuffer {
    uint8_t *data;
    uint32_t size;
    uint32_t startIndex;
    IndexBufferHandle handle;
    bool isIndex16;
};

struct TransientVertexBuffer {
    uint8_t *data;
    uint32_t size;
    uint32_t startVertex;
    uint16_t stride;
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
};

}