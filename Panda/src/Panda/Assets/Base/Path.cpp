#include "Panda/Assets/Base/Path.hpp"

namespace Panda {

path_t replaceLastChars(const path_t &path, const std::string &replacement) {
    std::string str = path.string();
    if (str.size() < replacement.size()) { return path; }
    for (int i = 0; i < replacement.size(); i++) {
        str[str.size() - i - 1] = replacement[replacement.size() - i - 1];
    }
    return path_t(str);
}

} // namespace Panda