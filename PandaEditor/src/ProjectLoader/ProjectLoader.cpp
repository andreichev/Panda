#include "ProjectLoader.hpp"
#include "Panda/Serialization/WorldMapper.hpp"

#include <fstream>
#include <sstream>

namespace Panda {

ProjectLoader::ProjectLoader(ProjectLoaderOutput *output)
    : m_output(output)
    , jsonEncoder(true)
    , jsonDecoder()
    , m_editorSettings()
    , m_projectSettings()
    , m_projectPath()
    , m_worldPath() {}

ProjectLoader::~ProjectLoader() {
    saveAppSettings();
    saveProjectSettings();
}

void ProjectLoader::loadInitialData() {
    // Load general settings
    {
        std::ifstream file("config.json");
        if (file.is_open()) {
            Rain::Decoder *decoder = &jsonDecoder;
            decoder->decode(file, m_editorSettings);
            file.close();
        } else {
            LOG_INFO("GENERAL SETTINGS NOT FOUND");
        }
    }
    if (m_editorSettings.hasOpenedProject) {
        loadRecentProject();
    }
}

void ProjectLoader::saveAppSettings() {
    // Save general settings
    appendRecentProject();
    m_editorSettings.hasOpenedProject = !m_projectPath.empty();
    std::ofstream file("config.json");
    if (file.is_open()) {
        Rain::Encoder *encoder = &jsonEncoder;
        encoder->encode(file, m_editorSettings);
        file.close();
    }
}

void ProjectLoader::loadRecentProject() {
    if (m_editorSettings.recentProjects.empty()) {
        return;
    }
    auto &recentProject = m_editorSettings.recentProjects.front();
    openProject(recentProject.path);
}

void ProjectLoader::openProject(const path_t &path) {
    m_projectPath = path;
    m_worldPath.clear();
    path_t pandaDirectoryPath = path;
    pandaDirectoryPath.append(".Panda");
    if (!std::filesystem::exists(pandaDirectoryPath)) {
        std::filesystem::create_directory(pandaDirectoryPath);
    }
    path_t projectConfigPath = pandaDirectoryPath;
    projectConfigPath.append("project.json");
    // Load project settings
    {
        std::ifstream file(projectConfigPath);
        if (file.is_open()) {
            Rain::Decoder *decoder = &jsonDecoder;
            decoder->decode(file, m_projectSettings);
            file.close();
        } else {
            LOG_INFO("PROJECT SETTINGS NOT FOUND");
        }
    }
    m_output->loaderDidLoadProject();
    if(m_projectSettings.worldPath.empty()) {
        return;
    }
    m_worldPath = m_projectPath;
    m_worldPath.append(m_projectSettings.worldPath);
    loadWorld();
}

bool ProjectLoader::hasOpenedProject() {
    return !m_projectPath.empty();
}

void ProjectLoader::createProject(const path_t &path) {
    std::filesystem::create_directory(path);
    // TODO: Add necessary files
    openProject(path);
}

void ProjectLoader::appendRecentProject() {
    if (m_projectPath.empty()) {
        return;
    }
    RecentProject recentProject;
    recentProject.path = m_projectPath;
    recentProject.name = m_projectPath.filename();
    auto &recentList = m_editorSettings.recentProjects;
    auto existing = std::find(recentList.begin(), recentList.end(), recentProject);
    if (existing != recentList.end()) {
        // Move existing recent project to last
        std::rotate(recentList.begin(), existing, existing + 1);
        return;
    }
    m_editorSettings.recentProjects.push_back(recentProject);
}

const std::vector<RecentProject> &ProjectLoader::getRecentProjectsList() {
    return m_editorSettings.recentProjects;
}

void ProjectLoader::saveWorld(const World &world) {
    if (m_worldPath.empty()) {
        saveWorldAs(world);
        return;
    }
    WorldDto worldDto = WorldMapper::toDto(world);
    std::ofstream file(m_worldPath);
    if (file.is_open()) {
        Rain::Encoder *encoder = &jsonEncoder;
        encoder->encode(file, worldDto);
        file.close();
    } else {
        LOG_ERROR("ERROR SAVING WORLD AT PATH {}", m_worldPath.string());
    }
}

void ProjectLoader::loadWorld() {
    if(m_worldPath.empty()) {
        return;
    }
    WorldDto worldDto;
    std::ifstream file(m_worldPath);
    if (file.is_open()) {
        Rain::Decoder *decoder = &jsonDecoder;
        decoder->decode(file, worldDto);
        file.close();
    } else {
        LOG_INFO("CANNOT LOAD WORLD AT PATH {}", m_worldPath.string());
    }
    World world;
    WorldMapper::fillWorld(world, worldDto);
    m_output->loaderDidLoadWorld(world);
}

void ProjectLoader::saveWorldAs(const World &world) {
    std::optional<path_t> optionalPath =
        FileSystem::saveFileDialog("All\0*.pnd\0", nullptr, "world.pnd");
    if (!optionalPath.has_value()) {
        return;
    }
    m_worldPath = optionalPath.value();
    saveWorld(world);
}

void ProjectLoader::closeProject() {
    saveAppSettings();
    saveProjectSettings();
    m_projectSettings.clear();
    m_worldPath.clear();
    m_projectPath.clear();
    m_output->loaderDidLoadCloseProject();
}

void ProjectLoader::removeRecentProject(int index) {
    LOG_INFO("REMOVE PROJECT AT INDEX {}", index);
    m_editorSettings.recentProjects.erase(std::next(m_editorSettings.recentProjects.begin(), index)
    );
}

const ProjectSettings &ProjectLoader::getProjectSettings() {
    return m_projectSettings;
}

void ProjectLoader::saveProjectSettings() {
    if (m_projectPath.empty()) {
        return;
    }
    path_t pandaDirectoryPath = m_projectPath;
    pandaDirectoryPath.append(".Panda");
    if (!std::filesystem::exists(pandaDirectoryPath)) {
        std::filesystem::create_directory(pandaDirectoryPath);
    }
    path_t projectConfigPath = pandaDirectoryPath;
    projectConfigPath.append("project.json");
    path_t worldPath = std::filesystem::relative(m_projectPath, m_worldPath);
    // Save project settings
    {
        std::ofstream file(projectConfigPath);
        if (file.is_open()) {
            Rain::Encoder *encoder = &jsonEncoder;
            encoder->encode(file, m_projectSettings);
            file.close();
        } else {
            LOG_INFO("PROJECT SETTINGS SAVING ERROR");
        }
    }
}

} // namespace Panda