#include "Memory.hpp"
#include "Allocator.hpp"

namespace Panda {

const Memory *allocateMemory(uint32_t size) {
    Memory *mem = (Memory *)PND_ALLOC(getAllocator(), sizeof(Memory) + size);
    mem->size = size;
    mem->data = (u_int8_t *)mem + sizeof(Memory);
    return mem;
}

const Memory *copy(const void *data, uint32_t size) {
    const Memory *mem = allocateMemory(size);
    memcpy(mem->data, data, size);
    return mem;
}

struct MemoryRef {
    Memory mem;
    ReleaseFn releaseFn;
    void *userData;
};

const Memory *makeRef(const void *data, uint32_t size, ReleaseFn releaseFn, void *userData) {
    MemoryRef *memRef = (MemoryRef *)PND_ALLOC(getAllocator(), sizeof(MemoryRef));
    memRef->mem.size = size;
    memRef->mem.data = (uint8_t *)data;
    memRef->releaseFn = releaseFn;
    memRef->userData = userData;
    return &memRef->mem;
}

bool isMemoryRef(const Memory *_mem) {
    return _mem->data != (uint8_t *)_mem + sizeof(Memory);
}

void release(const Memory *_mem) {
    Memory *mem = const_cast<Memory *>(_mem);
    if (isMemoryRef(mem)) {
        MemoryRef *memRef = reinterpret_cast<MemoryRef *>(mem);
        if (NULL != memRef->releaseFn) {
            memRef->releaseFn(mem->data, memRef->userData);
        }
    }
    PND_FREE(getAllocator(), mem);
}

} // namespace Panda