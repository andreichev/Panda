#pragma once

#include "Panda/Assets/Texture.hpp"
#include "SystemTools/SystemTools.hpp"

#include <filesystem>
#include <map>

namespace Panda {

class ContentBrowserOutput {
public:
    virtual ~ContentBrowserOutput() = default;
    virtual void showCreateFolderPopup() = 0;
    virtual void deleteFileShowPopup(path_t path) = 0;
    virtual void importAsset(const path_t &path) = 0;
    virtual UUID getAssetId(const path_t &path) = 0;
};

class ContentBrowser {
public:
    ContentBrowser(ContentBrowserOutput *output);
    void onImGuiRender();
    void setBaseDirectory(const path_t &path);
    void createFolder(std::string name);
    void confirmDeletion();

private:
    path_t m_baseDirectory;
    path_t m_currentDirectory;
    path_t m_deletingDirectory;
    Texture m_directoryIcon;
    Texture m_defaultFileIcon;
    Texture m_importedIcon;
    ContentBrowserOutput *m_output;
    std::map<std::string, Texture> m_fileIcons;
};

} // namespace Panda