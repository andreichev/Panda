#pragma once

#include "StreamReaderI.hpp"

namespace Foundation {

class MemoryStreamReader final : public StreamReaderI {
public:
    MemoryStreamReader(const void *data, size_t size);
    MemoryStreamReader(const MemoryStreamReader &) = delete;
    ~MemoryStreamReader();

    size_t totalSize() const override {
        return m_size;
    }
    bool isStreamGood() const override {
        return m_readPos < m_size;
    }
    uint64_t getStreamPosition() override {
        return m_readPos;
    }
    void setStreamPosition(uint64_t position) override {
        m_readPos = position;
    }
    bool readData(void *destination, size_t size) override;

private:
    const void *m_data;
    size_t m_size;
    size_t m_readPos;
};

} // namespace Foundation
