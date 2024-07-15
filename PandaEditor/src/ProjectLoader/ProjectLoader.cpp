#include "ProjectLoader.hpp"

#include <fstream>
#include <sstream>

namespace Panda {

ProjectLoader::ProjectLoader()
    : jsonEncoder(true)
    , jsonDecoder()
    , m_config()
    , m_projectPath()
    , m_worldPath() {
    // Load general settings
    {
        std::ifstream file("config.json");
        if (file.is_open()) {
            Rain::Decoder *decoder = &jsonDecoder;
            decoder->decode(file, m_config);
            file.close();
        } else {
            LOG_INFO("GENERAL SETTINGS NOT FOUND");
        }
    }
}

ProjectLoader::~ProjectLoader() {
    // Save general settings
    {
        appendRecentProject();
        std::ofstream file("config.json");
        if (file.is_open()) {
            Rain::Encoder *encoder = &jsonEncoder;
            encoder->encode(file, m_config);
            file.close();
        }
    }
}

void ProjectLoader::openProject(const std::filesystem::path &path) {
    m_projectPath = path;
}

std::optional<World> ProjectLoader::openWorld(const std::filesystem::path &path) {
    m_worldPath = path;
    return {};
}

bool ProjectLoader::hasOpenedWorld() {
    return !m_worldPath.empty();
}

bool ProjectLoader::hasOpenedProject() {
    return !m_projectPath.empty();
}

void ProjectLoader::createProject(const std::filesystem::path &path) {
    std::filesystem::create_directory(path);
    m_projectPath = path;
}

void ProjectLoader::appendRecentProject() {
    if (m_projectPath.empty()) {
        return;
    }
    RecentProject recentProject;
    recentProject.path = m_projectPath;
    recentProject.name = m_projectPath.filename();
    auto &recentList = m_config.recentProjects;
    auto existing = std::find(recentList.begin(), recentList.end(), recentProject);
    if (existing != recentList.end()) {
        // Move existing recent project to last
        std::rotate(recentList.begin(), existing, existing + 1);
        return;
    }
    m_config.recentProjects.push_back(recentProject);
}

const std::vector<RecentProject> &ProjectLoader::getRecentProjectsList() {
    return m_config.recentProjects;
}

} // namespace Panda