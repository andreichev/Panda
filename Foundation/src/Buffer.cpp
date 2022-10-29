#include "Foundation/Buffer.hpp"
#include "Foundation/Allocator.hpp"

namespace Panda {

const Buffer *allocateBuffer(uint32_t size) {
    Buffer *buffer = (Buffer *)PND_ALLOC(getAllocator(), sizeof(Buffer) + size);
    buffer->size = size;
    buffer->data = (u_int8_t *)buffer + sizeof(Buffer);
    return buffer;
}

void releaseBuffer(const Buffer *_buffer) {
    Buffer *buffer = const_cast<Buffer *>(_buffer);
    PND_FREE(getAllocator(), buffer);
}

} // namespace Panda