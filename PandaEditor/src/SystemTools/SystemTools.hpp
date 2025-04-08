#pragma once

#include <Panda/Assets/Path.hpp>
#include <optional>

namespace Panda {

class SystemTools {
public:
    static std::optional<path_t> openFileDialog(const char *filter = "All\0*.*\0");
    static std::optional<path_t> openFolderDialog(const char *initialFolder = "");
    static std::optional<path_t> saveFileDialog(
        const char *filter = "All\0*.*\0",
        const char *defaultPath = nullptr,
        const char *defaultName = nullptr
    );
    static void copyFolder(const path_t &sourcePath, const path_t &newPath);
    static void open(const path_t &path);
    static void show(const path_t &path);
    static void openCppProject(const path_t &path);
};

} // namespace Panda