#pragma once

#include "Panda/Renderer/Texture.hpp"
#include "SystemTools/SystemTools.hpp"

#include <filesystem>
#include <map>

namespace Panda {

class ContentBrowser {
public:
    ContentBrowser();
    void onImGuiRender();
    void setBaseDirectory(const path_t &path);

private:
    path_t m_baseDirectory;
    path_t m_currentDirectory;
    Texture m_directoryIcon;
    Texture m_defaultFileIcon;
    std::map<std::string, Texture> m_fileIcons;
};

} // namespace Panda