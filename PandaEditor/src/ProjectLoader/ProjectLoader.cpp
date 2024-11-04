#include "ProjectLoader.hpp"
#include "Panda/Serialization/WorldMapper.hpp"

#include <fstream>
#include <sstream>

namespace Panda {

bool isSubpath(const path_t &path, const path_t &base) {
    auto rel = std::filesystem::relative(path, base);
    return !rel.empty() && rel.native()[0] != '.';
}

ProjectLoader::ProjectLoader(World *world, ProjectLoaderOutput *output)
    : m_output(output)
    , m_world(world)
    , m_jsonEncoder(true)
    , m_jsonDecoder()
    , m_editorSettings()
    , m_projectSettings()
    , m_projectPath()
    , m_worldPath()
    , m_assetHandler() {}

ProjectLoader::~ProjectLoader() {
    saveAppSettings();
    saveProjectSettings();
}

void ProjectLoader::loadInitialData() {
    // Load general settings
    {
        std::ifstream file("config.json");
        if (file.is_open()) {
            Rain::Decoder *decoder = &m_jsonDecoder;
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
    m_editorSettings.hasOpenedProject = !m_projectPath.empty();
    std::ofstream file("config.json");
    if (file.is_open()) {
        Rain::Encoder *encoder = &m_jsonEncoder;
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
    if (!std::filesystem::exists(path)) {
        LOG_INFO("PROJECT NOT FOUND AT PATH {}", path.string());
        return;
    }
    m_projectPath = path;
    path_t projectName = path.filename();
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
            Rain::Decoder *decoder = &m_jsonDecoder;
            decoder->decode(file, m_projectSettings);
            file.close();
        } else {
            m_projectSettings.clear();
            LOG_INFO("PROJECT SETTINGS NOT FOUND");
        }
    }
    // Load editor camera location
    { m_output->setEditorCameraSettings(m_projectSettings.editorCameraSettings); }
    m_output->loaderDidLoadProject(projectName.string(), m_projectPath);
    LOG_INFO("LOADED PROJECT AT PATH {}", m_projectPath.string());
    if (m_projectSettings.worldPath.empty()) {
        m_output->loaderCreateSampleWorld();
        return;
    }
    m_worldPath = m_projectPath;
    m_worldPath.append(m_projectSettings.worldPath);
    reloadScriptsDll();
    loadWorld();
    appendRecentProject();
}

void ProjectLoader::reloadScriptsDll() {
    std::string projectName = m_projectPath.filename().string();
    m_scriptEngine.reload({m_projectPath / "Scripts" / projectName / "bin", projectName});
    m_world->rebindScriptsAndFields();
}

bool ProjectLoader::hasOpenedProject() {
    return !m_projectPath.empty();
}

const path_t &ProjectLoader::getOpenedProjectPath() {
    return m_projectPath;
}

void ProjectLoader::createProject(const std::string &name, const path_t &path) {
    m_projectCreator.createProject(name, path);
    openProject(path);
}

void ProjectLoader::openCppProject() {
    std::string name = m_projectPath.filename().string();
    SystemTools::openCppProject(m_projectPath / "Scripts" / name);
}

void ProjectLoader::appendRecentProject() {
    if (m_projectPath.empty()) {
        return;
    }
    RecentProject recentProject;
    recentProject.path = m_projectPath.string();
    recentProject.name = m_projectPath.filename().string();
    auto &recentList = m_editorSettings.recentProjects;
    auto existing = std::find(recentList.begin(), recentList.end(), recentProject);
    if (existing != recentList.end()) {
        // Move existing recent project to last
        std::rotate(recentList.begin(), existing, existing + 1);
        return;
    }
    recentList.insert(recentList.begin(), recentProject);
}

const std::vector<RecentProject> &ProjectLoader::getRecentProjectsList() {
    return m_editorSettings.recentProjects;
}

void ProjectLoader::saveWorld() {
    if (m_worldPath.empty()) {
        saveWorldAs();
        return;
    }
    WorldDto worldDto = WorldMapper::toDto(*m_world);
    std::ofstream file(m_worldPath);
    if (file.is_open()) {
        Rain::Encoder *encoder = &m_jsonEncoder;
        encoder->encode(file, worldDto);
        file.close();
        m_world->setChanged(false);
    } else {
        LOG_ERROR("ERROR SAVING WORLD AT PATH {}", m_worldPath.string());
    }
}

void ProjectLoader::loadWorld() {
    if (m_worldPath.empty()) {
        m_output->loaderCreateSampleWorld();
        return;
    }
    WorldDto worldDto;
    std::ifstream file(m_worldPath);
    if (file.is_open()) {
        Rain::Decoder *decoder = &m_jsonDecoder;
        decoder->decode(file, worldDto);
        file.close();
    } else {
        LOG_INFO("CANNOT LOAD WORLD AT PATH {}", m_worldPath.string());
        m_worldPath.clear();
        m_projectSettings.worldPath.clear();
        m_output->loaderCreateSampleWorld();
        return;
    }
    WorldMapper::fillWorld(*m_world, worldDto);
    LOG_INFO("LOADED WORLD AT PATH {}", m_worldPath.string());
    m_output->loaderDidLoadWorld();
}

void ProjectLoader::saveWorldAs() {
    std::optional<path_t> optionalPath = SystemTools::saveFileDialog(
        "All\0*.pnd\0", (m_projectPath / "Assets").string().c_str(), "world.pnd"
    );
    if (!optionalPath.has_value()) {
        return;
    }
    m_worldPath = optionalPath.value();
    saveWorld();
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
    if (isSubpath(m_worldPath, m_projectPath)) {
        path_t worldPath = std::filesystem::relative(m_worldPath, m_projectPath);
        m_projectSettings.worldPath = worldPath.string();
    } else {
        m_projectSettings.worldPath.clear();
    }
    // Save editor camera location
    { m_projectSettings.editorCameraSettings = m_output->getEditorCameraSettings(); }
    // Save project settings
    {
        std::ofstream file(projectConfigPath);
        if (file.is_open()) {
            Rain::Encoder *encoder = &m_jsonEncoder;
            encoder->encode(file, m_projectSettings);
            file.close();
        } else {
            LOG_INFO("PROJECT SETTINGS SAVING ERROR");
        }
    }
}

} // namespace Panda