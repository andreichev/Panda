#include "EditorLayer.hpp"

#include "Panda/GameLogic/GameContext.hpp"
#include "UI/Common/ImGuiHelper.hpp"
#include "UI/Popups/EditorYesNoPopup.hpp"
#include "UI/Popups/PickScriptPopup.hpp"
#include "UI/Popups/EnterNamePopup.hpp"
#include "UI/Popups/EditorAboutPopup.hpp"
#include "Panda/WorldCommands/Impl/AddRemoveEntitiesCommand.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Fern/Fern.hpp>
#include <Fern/Events/WindowEvents.hpp>

namespace Panda {

EditorLayer::EditorLayer(Fern::Window *window)
    : m_editingWorld()
    , m_playingWorld()
    , m_currentWorld(&m_editingWorld)
    , m_viewportFullscreen(false)
    , m_panelsContainer(this, &m_cameraController)
    , m_loader(&m_editingWorld, this)
    , m_startPanel(&m_loader)
    , m_editorCamera()
    , m_popups()
    , m_cameraController()
    , m_sceneState(SceneState::EDIT)
    , m_grid(&m_cameraController)
    , m_window(window) {}

void EditorLayer::onAttach() {
    Foundation::EditorLogger::init(ConsolePanel::loggerCallback);
    GameContext::s_currentWorld = m_currentWorld;
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    Fern::Size windowSize = Application::get()->getMainWindow()->getSize();
    // Default framebuffer initial size
    Miren::setViewport(
        0, Miren::Rect(0, 0, windowSize.width * dpi.width, windowSize.height * dpi.height)
    );
    m_panelsContainer.viewport.initWithSize(Size(100.f, 100.f));
    m_editingWorld.setViewId(m_panelsContainer.viewport.getMirenView());
    m_playingWorld.setViewId(m_panelsContainer.viewport.getMirenView());
    m_grid.setViewId(m_panelsContainer.viewport.getMirenView());
    m_panelsContainer.viewport.setCamera(&m_editorCamera);
    m_panelsContainer.setCurrentWorld(m_currentWorld);
    m_cameraController.setPosition({0.f, 0.f, 4.f});
    m_loader.loadInitialData();
    LOG_INFO_EDITOR("EDITOR INITIALIZED");
}

void EditorLayer::onDetach() {
    GameContext::s_currentWorld = nullptr;
}

void EditorLayer::onUpdate(double deltaTime) {
    processShortcuts();
    if (!m_loader.hasOpenedProject()) { return; }
    switch (m_sceneState) {
        case SceneState::EDIT: {
            m_cameraController.update(deltaTime);
            glm::mat4 view = m_cameraController.getViewMatrix();
            glm::mat4 proj = m_editorCamera.getProjection();
            glm::mat4 cameraViewProj = proj * m_cameraController.getSkyViewMatrix();
            glm::mat4 viewProj = proj * view;
            m_editingWorld.updateEditor(deltaTime, viewProj, cameraViewProj);
            m_grid.update(viewProj);
            break;
        }
        case SceneState::PLAY: {
            m_panelsContainer.viewport.setCamera(m_playingWorld.findMainCamera());
            m_playingWorld.updateRuntime(deltaTime);
            break;
        }
        case SceneState::SIMULATE: {
            m_cameraController.update(deltaTime);
            glm::mat4 view = m_cameraController.getViewMatrix();
            glm::mat4 proj = m_editorCamera.getProjection();
            glm::mat4 cameraViewProj = proj * m_cameraController.getSkyViewMatrix();
            glm::mat4 viewProj = proj * view;
            m_playingWorld.updateSimulation(deltaTime, viewProj, cameraViewProj);
            m_grid.update(viewProj);
            break;
        }
    }
    m_panelsContainer.viewport.drawOutline();
}

void EditorLayer::onImGuiRender() {
    // ImGui::ShowDemoWindow();
    if (!m_popups.empty()) {
        auto &popup = m_popups.back();
        popup->onImGuiRender();
    }
    if (m_loader.hasOpenedProject()) {
        m_panelsContainer.onImGuiRender(m_viewportFullscreen, m_sceneState);
        m_cameraController.setRotationActive(
            (Input::isTrackpadScroll() && m_panelsContainer.viewport.isHovered()) ||
            m_panelsContainer.viewport.isFocused()
        );
        m_cameraController.setMoveActive(
            (m_panelsContainer.viewport.isHovered() && !ImGui::IsAnyItemActive()) ||
            m_panelsContainer.viewport.isFocused()
        );
    } else {
        m_startPanel.onImGuiRender();
    }
}

void EditorLayer::onEvent(Fern::Event *event) {
    if (event->type == Fern::EventType::QuitRequest ||
        event->type == Fern::EventType::WindowCloseRequest) {
        event->isHandled = true;
        closeApp();
    } else if (event->type == Fern::EventType::WindowResize) {
        const Fern::WindowResizeEvent *ev = static_cast<const Fern::WindowResizeEvent *>(event);
        windowSizeChanged(Size(ev->getWidth(), ev->getHeight()));
    }
}

void EditorLayer::windowSizeChanged(Size size) {
    Miren::Rect viewport = Miren::Rect(
        0, 0, size.width * m_window->getDpi().width, size.height * m_window->getDpi().height
    );
    Miren::setViewport(0, viewport);
#ifdef PLATFORM_DESKTOP
    m_window->getDrawingContext()->update();
#endif
}

void EditorLayer::play() {
    m_sceneState = SceneState::PLAY;
    m_playingWorld = m_editingWorld;
    m_currentWorld = &m_playingWorld;
    GameContext::s_currentWorld = m_currentWorld;
    m_panelsContainer.setCurrentWorld(m_currentWorld);
    m_playingWorld.startRunning();
    m_panelsContainer.viewport.focus();
}

void EditorLayer::simulate() {
    m_sceneState = SceneState::SIMULATE;
    m_playingWorld = m_editingWorld;
    m_currentWorld = &m_playingWorld;
    GameContext::s_currentWorld = m_currentWorld;
    m_panelsContainer.setCurrentWorld(m_currentWorld);
    m_playingWorld.startRunning();
    m_panelsContainer.viewport.focus();
}

void EditorLayer::stop() {
    m_sceneState = SceneState::EDIT;
    m_panelsContainer.viewport.setCamera(&m_editorCamera);
    m_currentWorld = &m_editingWorld;
    GameContext::s_currentWorld = m_currentWorld;
    m_panelsContainer.setCurrentWorld(m_currentWorld);
    m_playingWorld.finishRunning();
    m_panelsContainer.viewport.focus();
}

#pragma region Project loader output

void EditorLayer::loaderDidLoadProject(const std::string &name, const path_t &path) {
    updateWindowState();
    m_panelsContainer.contentBrowser.setBaseDirectory(path / "Assets");
}

void EditorLayer::loaderDidLoadWorld() {
    m_window->setTitle(m_loader.getProjectSettings().worldPath.c_str());
}

void EditorLayer::loaderDidLoadCloseProject() {
    m_editingWorld.clear();
    m_playingWorld.clear();
    updateWindowState();
}

void EditorLayer::loaderCreateSampleWorld() {
    m_window->setTitle("Untitled World");
    m_editingWorld.clear();
    m_playingWorld.clear();
    m_editingWorld.fillStartupData();
}

EditorCameraSettings EditorLayer::getEditorCameraSettings() {
    EditorCameraSettings settings;
    settings.position = m_cameraController.getPosition();
    settings.rotation = m_cameraController.getRotation();
    return settings;
}

void EditorLayer::setEditorCameraSettings(EditorCameraSettings settings) {
    m_cameraController.setPosition(settings.position);
    m_cameraController.setRotation(settings.rotation);
}

#pragma endregion

#pragma region Menu bar output

bool EditorLayer::menuBarCanUndo() {
    return canUndo();
}

void EditorLayer::menuBarUndo() {
    undo();
}

bool EditorLayer::menuBarCanRedo() {
    return canRedo();
}

void EditorLayer::menuBarRedo() {
    redo();
}

void EditorLayer::menuBarOpenProject() {
    std::optional<path_t> optionalPath = SystemTools::openFolderDialog();
    if (!optionalPath.has_value()) { return; }
    path_t path = optionalPath.value();
    m_loader.openProject(path);
}

void EditorLayer::menuBarOpenProject(const RecentProject &project) {
    if (m_editingWorld.isChanged()) {
        EditorYesNoPopup *popup = F_NEW(Foundation::getAllocator(), EditorYesNoPopup);
        popup->yesAction = [&]() {
            saveWorld();
            m_loader.openProject(project.path);
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->noAction = [&]() {
            m_loader.openProject(project.path);
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->closeAction = [&]() {
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->title = "Save current world?";
        popup->subtitle = "Do you want to save your changes?";
        popup->yesText = "Save";
        popup->noText = "Not save";
        m_popups.emplace_back(popup);
    } else {
        m_loader.openProject(project.path);
    }
}

const std::vector<RecentProject> &EditorLayer::menuBarGetRecentProjectsList() {
    return m_loader.getRecentProjectsList();
}

const path_t &EditorLayer::menuBarGetOpenedProjectPath() {
    return m_loader.getOpenedProjectPath();
}

void EditorLayer::menuBarCloseApp() {
    if (m_editingWorld.isChanged()) {
        EditorYesNoPopup *popup = F_NEW(Foundation::getAllocator(), EditorYesNoPopup);
        popup->yesAction = [&]() {
            saveWorld();
            closeApp();
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->noAction = [&]() {
            closeApp();
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->closeAction = [&]() {
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->title = "Save current world?";
        popup->subtitle = "Do you want to save your changes?";
        popup->yesText = "Save";
        popup->noText = "Not save";
        m_popups.emplace_back(popup);
    } else {
        closeApp();
    }
}

void EditorLayer::menuBarOpenCppProject() {
    m_loader.openCppProject();
}

void EditorLayer::menuBarSaveWorld() {
    saveWorld();
}

void EditorLayer::menuBarCloseProject() {
    if (m_editingWorld.isChanged()) {
        EditorYesNoPopup *popup = F_NEW(Foundation::getAllocator(), EditorYesNoPopup);
        popup->yesAction = [&]() {
            saveWorld();
            closeProject();
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->noAction = [&]() {
            closeProject();
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->closeAction = [&]() {
            F_DELETE(Foundation::getAllocator(), m_popups.back());
            m_popups.pop_back();
        };
        popup->title = "Save current world?";
        popup->subtitle = "Do you want to save your changes?";
        popup->yesText = "Save";
        popup->noText = "Not save";
        m_popups.emplace_back(popup);
    } else {
        closeProject();
    }
}

void EditorLayer::menuBarAbout() {
    EditorAboutPopup *popup = F_NEW(Foundation::getAllocator(), EditorAboutPopup);
    popup->closeAction = [&]() {
        F_DELETE(Foundation::getAllocator(), m_popups.back());
        m_popups.pop_back();
    };
    popup->title = "About";
    m_popups.emplace_back(popup);
}

#pragma endregion

#pragma region Components draw output

void EditorLayer::addScriptToEntities(const std::vector<Entity> &entities) {
    PickScriptPopup *popup = F_NEW(Foundation::getAllocator(), PickScriptPopup);
    popup->closeAction = [&]() {
        F_DELETE(Foundation::getAllocator(), m_popups.back());
        m_popups.pop_back();
    };
    popup->selectAction = [&](const std::vector<Entity> &entities, ScriptClassManifest clazz) {
        if (clazz.name) {
            for (Entity entity : entities) {
                // Map manifest fields to internal ScriptField type
                std::vector<ScriptField> fields;
                for (auto manifestField : clazz.fields) {
                    Foundation::Memory data;
                    switch (manifestField.type) {
                        case ScriptFieldType::INTEGER: {
                            data = Foundation::Memory::alloc(sizeof(int));
                            break;
                        }
                        case ScriptFieldType::FLOAT: {
                            data = Foundation::Memory::alloc(sizeof(float));
                            break;
                        }
                        case ScriptFieldType::TEXTURE:
                        case ScriptFieldType::ENTITY: {
                            data = Foundation::Memory::alloc(sizeof(UUID));
                            break;
                        }
                        default: {
                            PND_ASSERT(false, "Unknown field type");
                        }
                    }
                    ScriptField field(
                        0, manifestField.handle, manifestField.name, manifestField.type, data
                    );
                    fields.emplace_back(field);
                }
                // TODO: Bind previously picked values
                entity.addScript(Panda::ExternalScript(0, clazz.name, fields));
            }
        }
        F_DELETE(Foundation::getAllocator(), m_popups.back());
        m_popups.pop_back();
    };
    popup->entities = entities;
    popup->title = "Add Script";
    popup->subtitle = "Pick a script from available Script classes list.";
    m_popups.emplace_back(popup);
}

#pragma endregion

#pragma region Content browser output

void EditorLayer::showCreateFolderPopup() {
    EnterNamePopup *popup = F_NEW(Foundation::getAllocator(), EnterNamePopup);
    popup->closeAction = [&]() {
        F_DELETE(Foundation::getAllocator(), m_popups.back());
        m_popups.pop_back();
    };
    popup->doneAction = [&](std::string text) {
        if (!text.empty()) { m_panelsContainer.contentBrowser.createFolder(text); }
        F_DELETE(Foundation::getAllocator(), m_popups.back());
        m_popups.pop_back();
    };
    popup->title = "Create folder";
    popup->subtitle = "Enter folder name:";
    m_popups.emplace_back(popup);
}

void EditorLayer::deleteFileShowPopup(path_t path) {
    EditorYesNoPopup *popup = F_NEW(Foundation::getAllocator(), EditorYesNoPopup);
    popup->yesAction = [&]() {
        m_panelsContainer.contentBrowser.confirmDeletion();
        F_DELETE(Foundation::getAllocator(), m_popups.back());
        m_popups.pop_back();
    };
    popup->noAction = [&]() {
        F_DELETE(Foundation::getAllocator(), m_popups.back());
        m_popups.pop_back();
    };
    popup->closeAction = popup->noAction;
    popup->title = "Delete";
    popup->subtitle = "Delete \"" + path.filename().string() + "\"?";
    popup->yesText = "Yes";
    popup->noText = "No";
    m_popups.emplace_back(popup);
}

void EditorLayer::importAsset(const path_t &path) {
    auto &assetHandler = m_loader.getAssetHandler();
    assetHandler.registerTextureAsset(path);
}

UUID EditorLayer::getAssetId(const path_t &path) {
    return m_loader.getAssetHandler().getAssetId(path);
}

#pragma endregion

#pragma region Toolbar output

bool EditorLayer::toolbarCanUndo() {
    return canUndo();
}

void EditorLayer::toolbarUndo() {
    undo();
}

bool EditorLayer::toolbarCanRedo() {
    return canRedo();
}

void EditorLayer::toolbarRedo() {
    redo();
}

void EditorLayer::toolbarDidPickSceneState(SceneState state) {
    switch (state) {
        case SceneState::EDIT: {
            stop();
            break;
        }
        case SceneState::PLAY: {
            play();
            break;
        }
        case SceneState::SIMULATE:
            simulate();
            break;
    }
    m_sceneState = state;
}

void EditorLayer::toolbarDidTapReloadScripts() {
    PND_ASSERT(
        m_sceneState == SceneState::EDIT,
        "RELOADING SCRIP ASSEMBLY NOT AVAILABLE WHEN WORLD IS RUNNING"
    );
    m_loader.reloadScriptsDll();
}

SceneState EditorLayer::toolbarGetCurrentSceneState() {
    return m_sceneState;
}

#pragma endregion

void EditorLayer::updateWindowState() {
    m_window->setResizable(m_loader.hasOpenedProject());
    if (m_loader.hasOpenedProject()) {
        auto windowState = m_loader.getLastWindowState();
        if (windowState.isFullScreen) {
            m_window->setState(Fern::WindowState::WindowStateFullScreen);
        } else {
            if (windowState.isSizeValid()) {
                m_window->setState(Fern::WindowState::WindowStateNormal);
                m_window->setSize({(float)windowState.width, (float)windowState.height});
            } else {
                m_window->setState(Fern::WindowState::WindowStateMaximized);
            }
        }
    } else {
        m_window->setTitle("Welcome");
        m_window->setState(Fern::WindowState::WindowStateNormal);
        m_window->setSize({600, 400});
    }
}

bool EditorLayer::canUndo() {
    if (!m_currentWorld) { return false; }
    return m_currentWorld->getCommandManger().CAN_UNDO();
}

void EditorLayer::undo() {
    if (!m_currentWorld) { return; }
    m_currentWorld->getCommandManger().UNDO();
}

bool EditorLayer::canRedo() {
    if (!m_currentWorld) { return false; }
    return m_currentWorld->getCommandManger().CAN_REDO();
}

void EditorLayer::redo() {
    if (!m_currentWorld) { return; }
    m_currentWorld->getCommandManger().REDO();
}

void EditorLayer::saveWorld() {
    switch (m_sceneState) {
        case SceneState::EDIT:
        case SceneState::SIMULATE:
            m_loader.saveWorld();
            break;
        case SceneState::PLAY:
            // TODO: Ask before overwriting
            // https://ru.yougile.com/team/91006f9f80d3/#PANDA-39
            m_editingWorld = m_playingWorld;
            m_loader.saveWorld();
            break;
    }
    LOG_INFO_EDITOR("World saved.");
}

void EditorLayer::closeProject() {
    Fern::WindowState windowState = m_window->getState();
    LastOpenedProjectWindowState savingState;
    savingState.isFullScreen = windowState == Fern::WindowState::WindowStateFullScreen;
    savingState.width = m_window->getSize().width;
    savingState.height = m_window->getSize().height;
    m_loader.saveWindowState(savingState);
    m_loader.closeProject();
}

void EditorLayer::closeApp() {
    Fern::WindowState windowState = m_window->getState();
    LastOpenedProjectWindowState savingState;
    savingState.isFullScreen = windowState == Fern::WindowState::WindowStateFullScreen;
    savingState.width = m_window->getSize().width;
    savingState.height = m_window->getSize().height;
    m_loader.saveWindowState(savingState);
    Application::get()->close();
}

void EditorLayer::processShortcuts() {
    bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
    bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
    if (ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        if (ctrl) { saveWorld(); }
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Z, false)) {
        if (ctrl) {
            if (shift) {
                redo();
            } else {
                undo();
            }
        } else {
            // Move to selected entity
            if (m_currentWorld) {
                SelectionContext &selectionContext = m_currentWorld->getSelectionContext();
                auto selected = selectionContext.getSelectedEntities();
                if (!selected.empty()) {
                    m_cameraController.reset(selectionContext.getMedianPosition());
                }
            }
        }
    }
    if (ImGui::IsKeyPressed(ImGuiKey_W, false) && ctrl) {
        m_viewportFullscreen = !m_viewportFullscreen;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_D, false) && ctrl) {
        SelectionContext &selectionContext = m_currentWorld->getSelectionContext();
        if (m_currentWorld) {
            std::vector<Entity> selected = selectionContext.getSelectedEntities();
            std::vector<Entity> copies;
            for (auto entity : selected) {
                Entity duplicate = m_currentWorld->duplicateEntity(entity);
                copies.push_back(duplicate);
                selectionContext.removeSelectedEntity(entity, false);
                selectionContext.addSelectedEntity(duplicate, false);
            }
            WorldCommandManager &cmd = m_currentWorld->getCommandManger();
            AddRemoveEntitiesCommand update(copies);
            cmd.SAVE(update, false);
            m_currentWorld->sort();
            m_currentWorld->setChanged();
        }
    }
}

} // namespace Panda
