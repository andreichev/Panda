#include "Foundation/Buffer.hpp"
#include "Foundation/Allocator.hpp"

namespace Foundation {

const Buffer *allocateBuffer(uint32_t size) {
    Buffer *buffer = (Buffer *)ALLOC(getAllocator(), sizeof(Buffer) + size);
    buffer->size = size;
    buffer->data = (uint8_t *)buffer + sizeof(Buffer);
    return buffer;
}

void releaseBuffer(const Buffer *_buffer) {
    Buffer *buffer = const_cast<Buffer *>(_buffer);
    FREE(getAllocator(), buffer);
}

} // namespace Foundation