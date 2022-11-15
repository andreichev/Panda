//
// Created by Admin on 17/04/2022.
//

#pragma once

#include <stdint.h>

namespace Miren {

constexpr uint16_t kInvalidHandle = UINT16_MAX;

class MirenHandleAllocator {
public:
    explicit MirenHandleAllocator(uint16_t maxHandles);
    ~MirenHandleAllocator();
    uint16_t getHandleAt(uint16_t at) const;
    uint16_t getNumHandles() const;
    uint16_t getMaxHandles() const;
    uint16_t alloc();
    bool isValid(uint16_t handle) const;
    void free(uint16_t handle);
    void reset();

private:
    uint16_t *m_dense;
    uint16_t *m_sparse;
    uint16_t m_numHandles;
    uint16_t m_maxHandles;
};

} // namespace Miren
