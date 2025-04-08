#include "Foundation/IO/MemoryStreamReader.hpp"

#include <memory>

namespace Foundation {

MemoryStreamReader::MemoryStreamReader(const void *data, size_t size)
    : m_data(data)
    , m_size(size)
    , m_readPos(0) {}

MemoryStreamReader::~MemoryStreamReader() {}

bool MemoryStreamReader::readData(void *destination, size_t size) {
    if (m_readPos + size > m_size) { return false; }
    const uint8_t *src = static_cast<const uint8_t *>(m_data) + m_readPos;
    memcpy(destination, src, size);
    m_readPos += size;
    return true;
}

} // namespace Foundation