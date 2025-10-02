#include "Foundation/IO/FileStreamReader.hpp"
#include "Foundation/Logger.hpp"

namespace Foundation {

FileStreamReader::FileStreamReader(const std::filesystem::path &path)
    : m_path(path) {
    m_stream = std::ifstream(path, std::ifstream::in | std::ifstream::binary);
    if (!m_stream.is_open()) {
        LOG_ERROR("FILE %s DOESN'T EXISTS", path.string().c_str());
        m_totalSize = 0;
        return;
    }
    m_totalSize = std::filesystem::file_size(path);
}

FileStreamReader::~FileStreamReader() {
    m_stream.close();
}

bool FileStreamReader::readData(void *destination, size_t size) {
    if (!m_stream.is_open()) { return false; }
    m_stream.read((char *)destination, size);
    return true;
}

} // namespace Foundation
