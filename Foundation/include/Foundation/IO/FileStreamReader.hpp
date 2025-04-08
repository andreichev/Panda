#pragma once

#include "StreamReaderI.hpp"

#include <filesystem>
#include <fstream>

namespace Foundation {

class FileStreamReader final : public StreamReaderI {
public:
    FileStreamReader(const std::filesystem::path &path);
    FileStreamReader(const FileStreamReader &) = delete;
    ~FileStreamReader();

    size_t totalSize() const override {
        return m_totalSize;
    }
    bool isStreamGood() const override {
        return m_stream.good();
    }
    uint64_t getStreamPosition() override {
        return m_stream.tellg();
    }
    void setStreamPosition(uint64_t position) override {
        m_stream.seekg(position);
    }
    bool readData(void *destination, size_t size) override;

private:
    size_t m_totalSize;
    std::filesystem::path m_path;
    std::ifstream m_stream;
};

} // namespace Foundation
