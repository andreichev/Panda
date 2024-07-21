
#include "ContentBrowser.hpp"
#include "Panda/ImGui/FontAwesome.h"

#include <imgui.h>

namespace Panda {

ContentBrowser::ContentBrowser()
    : m_baseDirectory()
    , m_currentDirectory()
    , m_defaultFileIcon("ui/icons/_plain.png")
    , m_directoryIcon("ui/icons/DirectoryIcon.png")
    , m_fileIcons() {
    m_fileIcons.emplace(".avi", Texture("ui/icons/avi.png"));
    m_fileIcons.emplace(".bmp", Texture("ui/icons/bmp.png"));
    m_fileIcons.emplace(".c", Texture("ui/icons/c.png"));
    m_fileIcons.emplace(".cpp", Texture("ui/icons/cpp.png"));
    m_fileIcons.emplace(".gif", Texture("ui/icons/gif.png"));
    m_fileIcons.emplace(".h", Texture("ui/icons/h.png"));
    m_fileIcons.emplace(".hpp", Texture("ui/icons/hpp.png"));
    m_fileIcons.emplace(".html", Texture("ui/icons/html.png"));
    m_fileIcons.emplace(".jpg", Texture("ui/icons/jpg.png"));
    m_fileIcons.emplace(".mp3", Texture("ui/icons/mp3.png"));
    m_fileIcons.emplace(".mp4", Texture("ui/icons/mp4.png"));
    m_fileIcons.emplace(".png", Texture("ui/icons/png.png"));
    m_fileIcons.emplace(".tga", Texture("ui/icons/tga.png"));
    m_fileIcons.emplace(".tiff", Texture("ui/icons/tiff.png"));
    m_fileIcons.emplace(".txt", Texture("ui/icons/txt.png"));
    m_fileIcons.emplace(".wav", Texture("ui/icons/wav.png"));
    m_fileIcons.emplace(".xml", Texture("ui/icons/xml.png"));
    m_fileIcons.emplace(".yml", Texture("ui/icons/yml.png"));
    m_fileIcons.emplace(".zip", Texture("ui/icons/zip.png"));
}

bool isMouseInsideWindow(ImVec2 windowPos, ImVec2 windowSize) {
    ImVec2 maxSize = windowPos;
    maxSize.x += windowSize.x;
    maxSize.y += windowSize.y;
    return ImGui::IsMouseHoveringRect(windowPos, maxSize);
}

void ContentBrowser::onImGuiRender() {
    if (m_currentDirectory.empty()) {
        return;
    }
    ImGui::Begin("Content Browser");
    if (m_currentDirectory != path_t(m_baseDirectory)) {
        if (ImGui::Button(getString(ICON_ARROW_LEFT).c_str())) {
            m_currentDirectory = m_currentDirectory.parent_path();
        }
    }
    static float padding = 8.0f;
    static float thumbnailSize = 90.0f;
    float cellSize = thumbnailSize + padding;

    float ContentBrowserWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(ContentBrowserWidth / cellSize);
    columnCount = std::max(columnCount, 1);
    // if (!Events::getDropPaths().empty()) {
    //     if (isMouseInsideWindow(ImGui::GetWindowPos(), ImGui::GetWindowSize())) {
    //         const auto &dropPaths = Events::getDropPaths();
    //         for (const auto &dropPath : dropPaths) {
    //             if (std::filesystem::is_directory(dropPath)) {
    //                 SystemTools::copyFolder(dropPath, m_currentDirectory.string());
    //                 LOG_INFO("COPY DIR THIS: {}, THERE: {}", dropPath,
    //                 m_currentDirectory.string());
    //             } else {
    //                 std::filesystem::copy(dropPath, m_currentDirectory);
    //                 LOG_INFO(
    //                     "COPY FILE THIS: {}, THERE: {}", dropPath, m_currentDirectory.string()
    //                 );
    //             }
    //         }
    //     }
    // }

    ImGui::Columns(columnCount, 0, false);
    for (auto &directoryEntry : std::filesystem::directory_iterator(m_currentDirectory)) {
        const auto &path = directoryEntry.path();
        std::string filenameString = path.filename().string();

        ImGui::PushID(filenameString.c_str());
        Texture *icon;
        if (directoryEntry.is_directory()) {
            icon = &m_directoryIcon;
        } else {
            if (m_fileIcons.find(path.extension().string()) != m_fileIcons.end()) {
                icon = &m_fileIcons[path.extension().string()];
            } else {
                icon = &m_defaultFileIcon;
            }
        }
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton(
            (ImTextureID)(intptr_t)icon->getHandle().id, {thumbnailSize, thumbnailSize}
        );
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (directoryEntry.is_directory()) {
                m_currentDirectory /= path.filename();
            }
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
            !is_directory(path)) {
            SystemTools::open(path);
        }
        ImGui::TextWrapped("%s", filenameString.c_str());
        ImGui::NextColumn();
        ImGui::PopID();
    }
    ImGui::Columns(1);
    // ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    // ImGui::SliderFloat("Padding", &padding, 0, 32);
    ImGui::End();
}

void ContentBrowser::setBaseDirectory(const path_t &path) {
    m_baseDirectory = path;
    m_currentDirectory = path;
}

} // namespace Panda