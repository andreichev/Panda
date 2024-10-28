#include "Panda/Serialization/FileStream.hpp"

namespace Panda {

FileStreamWriter::FileStreamWriter(const path_t &path)
    : m_path(path) {
    m_stream = std::ofstream(path, std::ifstream::out | std::ifstream::binary);
}

FileStreamWriter::~FileStreamWriter() {
    m_stream.close();
}

bool FileStreamWriter::writeData(const char *data, size_t size) {
    m_stream.write(data, size);
    return true;
}

FileStreamReader::FileStreamReader(const path_t &path)
    : m_path(path) {
    m_stream = std::ifstream(path, std::ifstream::in | std::ifstream::binary);
}

FileStreamReader::~FileStreamReader() {
    m_stream.close();
}

bool FileStreamReader::readData(char *destination, size_t size) {
    m_stream.read(destination, size);
    return true;
}

} // namespace Panda