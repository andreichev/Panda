#pragma once

#include <filesystem>

namespace Panda {

using path_t = std::filesystem::path;

path_t replaceLastChars(const path_t &path, const std::string &replacement);

} // namespace Panda