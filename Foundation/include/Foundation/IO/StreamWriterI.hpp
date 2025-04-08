#pragma once

#include <cstdio>
#include <cstdint>

namespace Foundation {

class StreamWriterI {
public:
    virtual ~StreamWriterI() = default;

    virtual bool isStreamGood() const = 0;
    virtual uint64_t getStreamPosition() = 0;
    virtual void setStreamPosition(uint64_t position) = 0;
    virtual bool writeData(const char *data, size_t size) = 0;

    operator bool() const {
        return isStreamGood();
    }
};

} // namespace Foundation
