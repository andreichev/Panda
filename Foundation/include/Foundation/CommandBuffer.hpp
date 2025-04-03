#pragma once

#include "Foundation/Allocator.hpp"
#include "Foundation/Assert.hpp"

#include <string>

#define COMMAND_BUFFER_ALIGNMENT 64

namespace Foundation {

class CommandBuffer {
    struct Header {
        uint32_t size;
        uint16_t align;

        Header(uint32_t size, uint16_t align)
            : size(size)
            , align(align) {}
    };

public:
    struct Command {
        uint16_t type;

        Command(uint16_t type)
            : type(type) {}

        Command()
            : type(0) {}
    };

    CommandBuffer(uint32_t size)
        : m_size(size)
        , m_readPos(0)
        , m_writePos(0) {
        m_data = (uint8_t *)F_ALLOC(getAllocator(), size);
    }

    ~CommandBuffer() {
        F_FREE(getAllocator(), m_data);
    }

    template<typename CMD>
    void write(CMD &cmd) {
        static_assert(std::is_base_of_v<Command, CMD>, "Not inherited from Command");
        Header header(sizeof(CMD), __alignof(CMD));
        writeHeader(header);
        align(m_writePos, __alignof(CMD));
        write(&cmd, sizeof(CMD));
    }

    Command *read() {
        if (m_readPos >= m_writePos) { return nullptr; }
        Header &header = readHeader();
        align(m_readPos, header.align);
        Command *cmd = (Command *)read(header.size);
        return cmd;
    }

    void reset() {
        m_readPos = 0;
        m_writePos = 0;
    }

private:
    void writeHeader(Header &header) {
        align(m_writePos, __alignof(Header));
        write(&header, sizeof(Header));
    }

    Header &readHeader() {
        align(m_readPos, __alignof(Header));
        return *(Header *)read(sizeof(Header));
    }

    void write(const void *_data, uint32_t _size) {
        memcpy(&m_data[m_writePos], _data, _size);
        m_writePos += _size;
        PND_ASSERT(m_writePos < m_size, "NEED MORE SPACE FOR BUFFER");
    }

    void *read(uint32_t _size) {
        uint32_t pos = m_readPos;
        m_readPos += _size;
        return &m_data[pos];
    }

    void align(uint32_t &_pos, uint32_t _alignment) {
        const uint32_t mask = _alignment - 1;
        const uint32_t pos = (_pos + mask) & (~mask);
        _pos = pos;
    }

    uint32_t m_size;
    uint32_t m_readPos;
    uint32_t m_writePos;
    uint8_t *m_data;
};

} // namespace Foundation
