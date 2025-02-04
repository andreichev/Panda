#include "StartPanel.hpp"
#include "Common/ImGuiHelper.hpp"

#include <optional>

namespace Panda {

StartPanel::StartPanel(ProjectLoader *loader)
    : m_logoImage("ui/start/Logo.png")
    , m_iconProject("ui/start/Icon Project.png")
    , m_newProjectMenu(false)
    , m_loader(loader) {}

void StartPanel::onImGuiRender() {
    // ImGui::ShowDemoWindow();
    ImGui::SetNextWindowPos({0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    auto size = Application::get()->getWindow()->getSize();
    auto wflags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration;
    ImGui::SetNextWindowSize({size.width, size.height});
    ImGui::Begin("Start panel", nullptr, wflags);
    // Left
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 25));
        ImGui::BeginChild("left pane", ImVec2(177, 0), true);
        ImGui::SetCursorPosX((177 - 50) / 2);
        ImGui::Image((void *)(uintptr_t)m_logoImage.getMirenHandle().id, {50, 50});
        const char *title = "Panda v 0.1";
        auto textWidth = ImGui::CalcTextSize(title).x;
        ImGui::SetCursorPosX((177 - textWidth) * 0.5f);
        ImGui::Text(title, nullptr);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 28);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5});
        if (!m_newProjectMenu) {
            if (ImGui::Button(
                    (getString(ICON_PLUS) + "  New Project").c_str(),
                    {ImGui::GetContentRegionAvail().x, 40}
                )) {
                m_newProjectMenu = true;
            }
        }
        if (ImGui::Button(
                (getString(ICON_FOLDER_OPEN) + "  Open Project").c_str(),
                {ImGui::GetContentRegionAvail().x, 40}
            )) {
            openProject();
        }
        ImGui::PopStyleVar(2);
        ImGui::EndChild();
        ImGui::PopStyleVar();
    }
    ImGui::SameLine();
    // Right
    {
        ImGui::BeginChild("right pane", ImVec2(0, -ImGui::GetFrameHeight()));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
        if (m_newProjectMenu) {
            static std::string name;
            if (name.size() < 40) { name.resize(40); }
            Fonts::pushFont("Bold");
            ImGui::TextColored({0.5, 0.5, 0.5, 1.0}, "New Project");
            Fonts::popFont();
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
            ImGui::Text("Enter new project name:");
            ImGui::InputText("##", name.data(), name.size());
            if (ImGui::Button("Cancel")) {
                m_newProjectMenu = false;
                name.clear();
            }
            ImGui::SameLine();
            if (ImGui::Button("Next")) {
                std::string clearName = name.substr(0, name.find('\0'));
                LOG_INFO("CREATE PROJECT WITH NAME {}", clearName);
                createProject(clearName);
                name.clear();
            }
        } else {
            Fonts::pushFont("Bold");
            ImGui::TextColored({0.5, 0.5, 0.5, 1.0}, "Recent Projects");
            Fonts::popFont();
            ImGui::Separator();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
            auto &recentProjects = m_loader->getRecentProjectsList();
            if (recentProjects.empty()) {
                Fonts::pushFont("Bold");
                ImGui::TextColored({0.5, 0.5, 0.5, 1.0}, "No recent projects");
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 18);
                if (ImGui::Button("Create New Project", {ImGui::GetContentRegionAvail().x, 40})) {
                    m_newProjectMenu = true;
                }
                Fonts::popFont();
            }
            for (int i = 0; i < recentProjects.size(); i++) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4);
                ImGui::Image((void *)(uintptr_t)m_iconProject.getMirenHandle().id, {40, 40});
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0, 0.5});
                if (ImGui::Button(
                        recentProjects[i].name.c_str(), {ImGui::GetContentRegionAvail().x - 55, 40}
                    )) {
                    m_loader->openProject(recentProjects[i].path);
                }
                ImGui::PopStyleVar();
                ImGui::SameLine();
                ImGui::PushID(("Delete " + recentProjects[i].name).c_str());
                if (ImGui::Button(getString(ICON_WINDOW_CLOSE).c_str(), {40, 40})) {
                    m_loader->removeRecentProject(i);
                }
                ImGui::PopID();
                ImGui::PopStyleColor();
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void StartPanel::openProject() {
    auto pathOptional = SystemTools::openFolderDialog();
    if (!pathOptional.has_value()) { return; }
    std::string path = pathOptional.value().string();
    LOG_INFO("OPEN PROJECT AT PATH {}", path);
    m_loader->openProject(path);
}

void StartPanel::createProject(const std::string &name) {
    m_newProjectMenu = false;
    auto pathOptional = SystemTools::openFolderDialog();
    if (!pathOptional.has_value()) { return; }
    path_t path = pathOptional.value();
    path.append(name);
    LOG_INFO("CREATE PROJECT AT PATH {}", path.string());
    m_loader->createProject(name, path);
}

} // namespace Panda