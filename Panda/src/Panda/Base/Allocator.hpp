#pragma once

#if DEBUG
#    define PND_ALLOC(allocator, size) Panda::alloc(allocator, _size, __FILE__, __LINE__)
#    define PND_FREE(allocator, ptr) Panda::free(allocator, ptr, __FILE__, __LINE__)
#    define PND_DELETE(_allocator, _ptr) Panda::deleteObject(_allocator, _ptr)
#else
#    define PND_ALLOC(allocator, size) Panda::alloc(allocator, size)
#    define PND_FREE(allocator, ptr) Panda::free(allocator, ptr)
#    define PND_DELETE(_allocator, _ptr) Panda::deleteObject(_allocator, _ptr, __FILE__, __LINE__)
#endif

#define PND_NEW(_allocator, _type) PND_PLACEMENT_NEW(PND_ALLOC(_allocator, sizeof(_type)), _type)
#define PND_PLACEMENT_NEW(_ptr, _type) ::new (Panda::PlacementNewTag(), _ptr) _type

namespace Panda {
struct PlacementNewTag {};
} // namespace Panda

void *operator new(size_t, Panda::PlacementNewTag, void *_ptr);
void operator delete(void *, Panda::PlacementNewTag, void *) throw();

namespace Panda {

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

} // namespace Panda