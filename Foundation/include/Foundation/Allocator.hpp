#pragma once

#include <cstdlib>

#if DEBUG
#    define ALLOC(allocator, size) Foundation::alloc(allocator, _size, __FILE__, __LINE__)
#    define FREE(allocator, ptr) Foundation::free(allocator, ptr, __FILE__, __LINE__)
#    define DELETE(_allocator, _ptr) Foundation::deleteObject(_allocator, _ptr)
#else
#    define ALLOC(allocator, size) Foundation::alloc(allocator, size)
#    define FREE(allocator, ptr) Foundation::free(allocator, ptr)
#    define DELETE(_allocator, _ptr) Foundation::deleteObject(_allocator, _ptr, __FILE__, __LINE__)
#endif

#define NEW(_allocator, _type) PLACEMENT_NEW(ALLOC(_allocator, sizeof(_type)), _type)
#define PLACEMENT_NEW(_ptr, _type) ::new (Foundation::PlacementNewTag(), _ptr) _type

namespace Foundation {
struct PlacementNewTag {};
} // namespace Foundation

void *operator new(size_t, Foundation::PlacementNewTag, void *_ptr);
void operator delete(void *, Foundation::PlacementNewTag, void *) throw();

namespace Foundation {

struct AllocatorI {
    virtual ~AllocatorI() = default;

    virtual void *realloc(void *ptr, size_t size, const char *file, uint32_t line) = 0;
};

class DefaultAllocator : public AllocatorI {
public:
    DefaultAllocator();

    ~DefaultAllocator() override;

    void *realloc(void *ptr, size_t size, const char *file, uint32_t line) override;
};

void *alloc(AllocatorI *allocator, size_t size, const char *file = NULL, uint32_t line = 0);

void free(AllocatorI *allocator, void *ptr, const char *file = NULL, uint32_t line = 0);

AllocatorI *getAllocator();

} // namespace Foundation