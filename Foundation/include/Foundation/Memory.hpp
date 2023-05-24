#pragma once

#include <cstdlib>

namespace Foundation {

typedef void (*ReleaseFunction)(void *ptr, void *userData);

struct Memory {
    Memory()
        : data(nullptr)
        , releaseFn(nullptr) {}

    Memory(void *data)
        : data(data)
        , releaseFn(nullptr) {}

    void *data;
    ReleaseFunction releaseFn;
    void *userData;

    const void release() const {
        if (releaseFn != nullptr) {
            (*releaseFn)(data, userData);
        }
    }
};

} // namespace Foundation