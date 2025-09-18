#pragma once

#include "Miren/Base.hpp"
#include "Miren/Encoder/PassInput.hpp"
#include "PassInput.hpp"

#include <cstdint>

namespace Miren {

class RenderPassInputs {
public:
    RenderPassInputs(uint32_t size)
        : m_size(size)
        , m_pos(0) {
        m_data = (uint8_t *)F_ALLOC(Foundation::getAllocator(), size);
    }

    RenderPassInputs(const RenderPassInputs &other)
        : m_size(other.m_size)
        , m_pos(other.m_pos)
        , m_data(other.m_data) {}

    RenderPassInputs(RenderPassInputs &&other) noexcept
        : m_size(other.m_size)
        , m_pos(other.m_pos)
        , m_data(other.m_data) {
        other.m_data = nullptr;
        other.m_pos = 0;
        other.m_size = 0;
    }

    RenderPassInputs &operator=(RenderPassInputs &&other) noexcept {
        if (this == &other) { return *this; }
        m_size = other.m_size;
        m_pos = other.m_pos;
        m_data = other.m_data;
        other.m_data = nullptr;
        return *this;
    }

    ~RenderPassInputs() {
        F_FREE(Foundation::getAllocator(), m_data);
    }

    void writeInput(PassInput &input) {
        align(__alignof(PassInput));
        PassInput *savedInput = (PassInput *)&m_data[m_pos];
        write(&input, sizeof(PassInput));
        if (input.data) {
            savedInput->alignment = alignof(std::max_align_t);
            align(savedInput->alignment);
            const void *data = savedInput->data;
            savedInput->data = &m_data[m_pos];
            write(data, savedInput->size);
        }
    }

    PassInput *readInput() {
        align(__alignof(PassInput));
        PassInput &input = *(PassInput *)read(sizeof(PassInput));
        if (m_pos == 0 || input.handle.id == MIREN_INVALID_HANDLE) { return nullptr; }
        align(input.alignment);
        skip(input.size);
        return &input;
    }

    // Finish writing. Start reading
    void finishWriting() {
        PassInput input(
            MIREN_INVALID_HANDLE, nullptr, RenderPassInputType::UniformBuffer, nullptr, 0
        );
        writeInput(input);
        m_pos = 0;
    }

    void reset() {
        m_pos = 0;
    }

private:
    void write(const void *_data, uint32_t _size) {
        memcpy(&m_data[m_pos], _data, _size);
        m_pos += _size;
        PND_ASSERT(m_pos < m_size, "NEED MORE SPACE FOR BUFFER");
    }

    void *read(uint32_t _size) {
        uint32_t pos = m_pos;
        m_pos += _size;
        return &m_data[pos];
    }

    void skip(uint32_t size) {
        m_pos += size;
    }

    void align(uint32_t _alignment) {
        const uint32_t mask = _alignment - 1;
        const uint32_t pos = (m_pos + mask) & (~mask);
        m_pos = pos;
    }

    uint32_t m_size;
    uint32_t m_pos;
    uint8_t *m_data;
};

} // namespace Miren