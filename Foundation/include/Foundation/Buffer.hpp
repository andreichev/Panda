#pragma once

#include <cstdlib>

namespace Foundation {

struct Buffer {
    Buffer() = delete;

    void *data;
    uint32_t size;
};

const Buffer *allocateBuffer(uint32_t size);

void releaseBuffer(const Buffer *buffer);

} // namespace Foundation