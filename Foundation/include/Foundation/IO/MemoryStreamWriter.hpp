#pragma once

#include "StreamWriterI.hpp"

namespace Foundation {

class MemoryStreamWriter final : public StreamWriterI {
public:
    MemoryStreamWriter(void *data, size_t size);
    MemoryStreamWriter(const MemoryStreamWriter &) = delete;
    ~MemoryStreamWriter();

    bool isStreamGood() const {
        return m_writePos < m_size;
    }
    uint64_t getStreamPosition() {
        return m_writePos;
    }
    void setStreamPosition(uint64_t position) {
        m_writePos = position;
    }
    bool writeData(const char *data, size_t size);

private:
    void *m_data;
    size_t m_size;
    size_t m_writePos;
};

} // namespace Foundation