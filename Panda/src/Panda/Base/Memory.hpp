#pragma once

namespace Panda {

typedef void (*ReleaseFn)(void *ptr, void *userData);

struct Memory {
    Memory() = delete;

    void *data;
    uint32_t size;
};

const Memory *allocateMemory(uint32_t size);

const Memory *copy(const void *_data, uint32_t _size);

const Memory *makeRef(const void *data, uint32_t size, ReleaseFn releaseFn = NULL, void *userData = NULL);

void release(const Memory *mem);

} // namespace Panda