#pragma once

#include "StreamWriterI.hpp"

#include <filesystem>
#include <fstream>

namespace Foundation {

class FileStreamWriter final : public StreamWriterI {
public:
    FileStreamWriter(const std::filesystem::path &path);
    FileStreamWriter(const FileStreamWriter &) = delete;
    virtual ~FileStreamWriter();

    bool isStreamGood() const {
        return m_stream.good();
    }
    uint64_t getStreamPosition() {
        return m_stream.tellp();
    }
    void setStreamPosition(uint64_t position) {
        m_stream.seekp(position);
    }
    bool writeData(const char *data, size_t size);

private:
    std::filesystem::path m_path;
    std::ofstream m_stream;
};

} // namespace Foundation