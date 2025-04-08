#include "Foundation/IO/MemoryStreamWriter.hpp"

#include <memory>

namespace Foundation {

MemoryStreamWriter::MemoryStreamWriter(void *data, size_t size)
    : m_data(data)
    , m_size(size)
    , m_writePos(0) {}

MemoryStreamWriter::~MemoryStreamWriter() {}

bool MemoryStreamWriter::writeData(const char *data, size_t size) {
    if (m_writePos + size > m_size) { return false; }
    uint8_t *dst = static_cast<uint8_t *>(m_data) + m_writePos;
    memcpy(dst, data, size);
    m_writePos += size;
    return true;
}

} // namespace Foundation