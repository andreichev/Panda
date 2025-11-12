#pragma once

#include <vector>
#include <cstdint>
#include <cstring>

namespace Panda {

class Std140Buffer final {
public:
    Std140Buffer(size_t initialSize = 1024)
        : m_currentOffset(0) {
        m_buffer.resize(initialSize);
    }

    void clear() {
        m_currentOffset = 0;
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
    }

    void resize(size_t newSize) {
        m_buffer.resize(newSize);
        if (m_currentOffset > newSize) { m_currentOffset = newSize; }
    }

    size_t getSize() const {
        return m_currentOffset;
    }

    const void *getData() const {
        return m_buffer.data();
    }

    void addFloat(float value) {
        alignOffset(4);
        ensureCapacity(4);
        std::memcpy(m_buffer.data() + m_currentOffset, &value, sizeof(float));
        m_currentOffset += sizeof(float);
    }

    void addVec2(float x, float y) {
        alignOffset(8);
        ensureCapacity(8);
        std::memcpy(m_buffer.data() + m_currentOffset, &x, sizeof(float));
        std::memcpy(m_buffer.data() + m_currentOffset + 4, &y, sizeof(float));
        m_currentOffset += 8;
    }

    void addVec3(float x, float y, float z) {
        alignOffset(16);
        ensureCapacity(16);
        std::memcpy(m_buffer.data() + m_currentOffset, &x, sizeof(float));
        std::memcpy(m_buffer.data() + m_currentOffset + 4, &y, sizeof(float));
        std::memcpy(m_buffer.data() + m_currentOffset + 8, &z, sizeof(float));
        m_currentOffset += 12;
        m_currentOffset += 4;
    }

    void addVec4(float x, float y, float z, float w) {
        alignOffset(16);
        ensureCapacity(16);
        std::memcpy(m_buffer.data() + m_currentOffset, &x, sizeof(float));
        std::memcpy(m_buffer.data() + m_currentOffset + 4, &y, sizeof(float));
        std::memcpy(m_buffer.data() + m_currentOffset + 8, &z, sizeof(float));
        std::memcpy(m_buffer.data() + m_currentOffset + 12, &w, sizeof(float));
        m_currentOffset += 16;
    }

    void addMat4(const float *matrix) {
        alignOffset(16);
        ensureCapacity(64);
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                size_t offset = m_currentOffset + col * 16 + row * 4;
                std::memcpy(m_buffer.data() + offset, &matrix[col * 4 + row], sizeof(float));
            }
        }
        m_currentOffset += 64;
    }

    void addInt(int value) {
        alignOffset(4);
        ensureCapacity(4);
        std::memcpy(m_buffer.data() + m_currentOffset, &value, sizeof(int));
        m_currentOffset += sizeof(int);
    }

    void addUInt(unsigned int value) {
        alignOffset(4);
        ensureCapacity(4);
        std::memcpy(m_buffer.data() + m_currentOffset, &value, sizeof(unsigned int));
        m_currentOffset += sizeof(unsigned int);
    }

private:
    std::vector<uint8_t> m_buffer;
    size_t m_currentOffset;

    void ensureCapacity(size_t required) {
        if (m_currentOffset + required > m_buffer.size()) {
            m_buffer.resize((m_currentOffset + required) * 2);
        }
    }

    size_t alignOffset(size_t alignment) {
        size_t padding = (alignment - (m_currentOffset % alignment)) % alignment;
        m_currentOffset += padding;
        return m_currentOffset;
    }
};

} // namespace Panda