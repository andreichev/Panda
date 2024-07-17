#include "FileSystem.hpp"

#include <Foundation/Logger.hpp>

#include <nfd.hpp>

namespace Panda {

std::optional<path_t> FileSystem::openFolderDialog(const char *initialFolder) {
    NFD::Init();
    nfdu8char_t *outPath;
    nfdresult_t result = NFD::PickFolder(outPath);
    std::optional<std::filesystem::path> res;
    if (result == NFD_OKAY) {
        res = outPath;
        NFD::FreePath(outPath);
    } else if (result == NFD_CANCEL) {
        LOG_INFO("User pressed cancel.");
    } else {
        LOG_ERROR("Error: %s", NFD::GetError());
    }
    NFD::Quit();
    return res;
}

std::optional<path_t> FileSystem::openFileDialog(const char *filter) {
    return {};
}

std::optional<path_t> FileSystem::saveFileDialog(const char *filter) {
    return {};
}

} // namespace Panda