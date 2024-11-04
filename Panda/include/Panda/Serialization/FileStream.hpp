#include <fstream>

#include "Panda/Base/Base.hpp"
#include "Panda/Base/FileBase.hpp"

namespace Panda {

class FileStreamWriter {
public:
    FileStreamWriter(const path_t &path);
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
    path_t m_path;
    std::ofstream m_stream;
};

class FileStreamReader {
public:
    FileStreamReader(const path_t &path);
    FileStreamReader(const FileStreamReader &) = delete;
    ~FileStreamReader();

    bool isStreamGood() const {
        return m_stream.good();
    }
    uint64_t getStreamPosition() {
        return m_stream.tellg();
    }
    void setStreamPosition(uint64_t position) {
        m_stream.seekg(position);
    }
    bool readData(char *destination, size_t size);

private:
    path_t m_path;
    std::ifstream m_stream;
};

} // namespace Panda