#include "Allocator.hpp"

#include <iostream>

inline void *operator new(size_t, Panda::PlacementNewTag, void *_ptr) {
    return _ptr;
}

inline void operator delete(void *, Panda::PlacementNewTag, void *) throw() {}

namespace Panda {

AllocatorI *getAllocator() {
    static DefaultAllocator allocator;
    return &allocator;
}

inline void *alloc(AllocatorI *allocator, size_t size, const char *file, uint32_t line) {
    return allocator->realloc(NULL, size, file, line);
}

inline void free(AllocatorI *allocator, void *ptr, const char *file, uint32_t line) {
    allocator->realloc(ptr, 0, file, line);
}

inline void *realloc(AllocatorI *allocator, void *ptr, size_t size, const char *file, uint32_t line) {
    return allocator->realloc(ptr, size, file, line);
}

DefaultAllocator::DefaultAllocator() {}

DefaultAllocator::~DefaultAllocator() {}

void *DefaultAllocator::realloc(void *_ptr, size_t _size, const char *_file, uint32_t _line) {
    if (0 == _size) {
        if (NULL != _ptr) {
            // System free memory call
            ::free(_ptr);
            return NULL;
        }
        return NULL;
    } else if (NULL == _ptr) {
        // System malloc call
        return ::malloc(_size);
    }
    // System realloc call
    return ::realloc(_ptr, _size);
}

} // namespace Panda