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
    loadData();
    loadRecentProject();
}

ProjectLoader::~ProjectLoader() {
    saveAppConfig();
}

void ProjectLoader::loadData() {
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

void ProjectLoader::saveAppConfig() {
    // Save general settings
    appendRecentProject();
    std::ofstream file("config.json");
    if (file.is_open()) {
        Rain::Encoder *encoder = &jsonEncoder;
        encoder->encode(file, m_config);
        file.close();
    }
}

void ProjectLoader::loadRecentProject() {
    if (m_config.recentProjects.empty()) {
        return;
    }
    auto &recentProject = m_config.recentProjects.front();
    openProject(recentProject.path);
}

void ProjectLoader::openProject(const path_t &path) {
    m_projectPath = path;
    path_t pandaDirectoryPath = path;
    pandaDirectoryPath.append(".Panda");
    if (!std::filesystem::exists(pandaDirectoryPath)) {
        std::filesystem::create_directory(pandaDirectoryPath);
    }
    path_t projectConfigPath = pandaDirectoryPath;
    projectConfigPath.append("project.json");
}

bool ProjectLoader::hasOpenedProject() {
    return !m_projectPath.empty();
}

void ProjectLoader::createProject(const path_t &path) {
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

void ProjectLoader::saveWorld(World *world) {
    if (m_worldPath.empty()) {
        saveWorldAs(world);
        return;
    }
}

void ProjectLoader::saveWorldAs(World *world) {}

} // namespace Panda