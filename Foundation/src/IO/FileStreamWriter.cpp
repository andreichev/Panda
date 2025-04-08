#include "Foundation/IO/FileStreamWriter.hpp"

namespace Foundation {

FileStreamWriter::FileStreamWriter(const std::filesystem::path &path)
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

} // namespace Foundation