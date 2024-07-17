#pragma once

#include <filesystem>

namespace Panda {

using path_t = std::filesystem::path;

class FileSystem {
public:
    static std::optional<path_t> openFileDialog(const char *filter = "All\0*.*\0");
    static std::optional<path_t> openFolderDialog(const char *initialFolder = "");
    static std::optional<path_t> saveFileDialog(const char *filter = "All\0*.*\0");
};

} // namespace Panda