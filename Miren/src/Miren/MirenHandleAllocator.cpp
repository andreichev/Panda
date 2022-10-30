//
// Created by Admin on 17/04/2022.
//

#include "MirenHandleAllocator.hpp"

namespace Miren {

MirenHandleAllocator::MirenHandleAllocator(uint16_t maxHandles)
    : m_numHandles(0)
    , m_maxHandles(maxHandles) {
    m_dense = new uint16_t[m_maxHandles];
    m_sparse = new uint16_t[m_maxHandles];
    reset();
}

MirenHandleAllocator::~MirenHandleAllocator() {
    delete[] m_dense;
    delete[] m_sparse;
}

uint16_t MirenHandleAllocator::getHandleAt(uint16_t at) const {
    return m_dense[at];
}

uint16_t MirenHandleAllocator::getNumHandles() const {
    return m_numHandles;
}

uint16_t MirenHandleAllocator::getMaxHandles() const {
    return m_maxHandles;
}

uint16_t MirenHandleAllocator::alloc() {
    if (m_numHandles < m_maxHandles) {
        uint16_t index = m_numHandles;
        ++m_numHandles;

        uint16_t handle = m_dense[index];
        m_sparse[handle] = index;
        return handle;
    }

    return kInvalidHandle;
}

bool MirenHandleAllocator::isValid(uint16_t handle) const {
    uint16_t index = m_sparse[handle];
    return index < m_numHandles && m_dense[index] == handle;
}

void MirenHandleAllocator::free(uint16_t handle) {
    uint16_t index = m_sparse[handle];
    --m_numHandles;
    uint16_t temp = m_dense[m_numHandles];
    m_dense[m_numHandles] = handle;
    m_sparse[temp] = index;
    m_dense[index] = temp;
}

void MirenHandleAllocator::reset() {
    m_numHandles = 0;
    for (uint16_t i = 0; i < m_maxHandles; ++i) {
        m_dense[i] = i;
    }
}

} // namespace Miren
