#pragma once

#include <cstdio>
#include <cstdint>

namespace Foundation {

class StreamReaderI {
public:
    virtual ~StreamReaderI() = default;

    virtual bool isStreamGood() const = 0;
    virtual uint64_t getStreamPosition() = 0;
    /// Returns an estimate of the number of bytes that can be read (or skipped over) from this
    /// input stream
    virtual size_t totalSize() const = 0;
    virtual void setStreamPosition(uint64_t position) = 0;
    virtual bool readData(void *destination, size_t size) = 0;

    operator bool() const {
        return isStreamGood();
    }
};

} // namespace Foundation
