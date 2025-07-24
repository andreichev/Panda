#include "ViewportPanel.hpp"

#include <Panda.hpp>
#include <imgui.h>
#include <Panda/ImGui/FontAwesome.h>
#include <Panda/GameLogic/GameContext.hpp>
#include <Panda/GameLogic/SelectionContext.hpp>

namespace Panda {

bool ViewportPanel::RectSelection::isEqualTo(IRect _rect) {
    return rect.size.width == _rect.size.width && rect.size.height == _rect.size.height;
}

ViewportPanel::ViewportPanel(CameraController *cameraController)
    : m_viewport()
    , m_rectSelection()
    , m_camera(nullptr)
    , m_cameraController(cameraController)
    , m_focused(false)
    , m_hovered(false)
    , m_focusNextFrame(true)
    , m_gizmos(nullptr, cameraController) {}

ViewportPanel::~ViewportPanel() {}

void ViewportPanel::initWithSize(Vec2 size) {
    m_viewport.initWithSize(size);
}

void ViewportPanel::updateViewportSize(Size size) {
    if (size.width < 1 || size.height < 1) { return; }
    if (m_camera) { m_camera->setViewportSize(size); }
    m_viewport.updateSize(size);
}

void ViewportPanel::drawOutline(float dt) {
    std::unordered_set<UUID> selection = SelectionContext::getSelectedEntities();
    m_viewport.drawOutline(dt, selection);
}

void ViewportPanel::onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen) {
    ImGuiWindowFlags window_flags = 0;
    if (fullScreen) {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 pos = viewport->WorkPos;
        pos.y += offsetY;
        ImGui::SetNextWindowPos(pos);
        ImVec2 size = viewport->WorkSize;
        size.y -= offsetY;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::Begin(fullScreen ? "Viewport (fullscreen mode)" : "Viewport", NULL, window_flags);
    ImVec2 viewportSpace = ImGui::GetContentRegionAvail();
    viewportSpace = ImVec2(viewportSpace.x - 4, viewportSpace.y - 4);
    if (m_viewport.getFrame().size != viewportSpace) { updateViewportSize(viewportSpace); }
    m_hovered = ImGui::IsWindowHovered();
    if ((m_hovered && Input::isMouseButtonPressed(Fern::MouseButton::RIGHT)) || m_focusNextFrame) {
        ImGui::SetWindowFocus();
        m_focusNextFrame = false;
    }
    m_focused = ImGui::IsWindowFocused();
    Application::get()->getImGuiLayer()->setBlockEvents(!m_hovered);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
    Vec2 viewportOrigin = ImGui::GetCursorScreenPos();
    m_viewport.updateOrigin(viewportOrigin);
    Input::setViewportFrame(m_viewport.getFrame());
    ImGui::Image(
        (void *)(uintptr_t)m_viewport.getResultTexture().id,
        viewportSpace,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );
    m_gizmos.onImGuiRender(sceneState, m_viewport.getFrame());

    ImGuiStyle &style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::SetCursorPos({10, 40});
    if (ImGui::Button(getString(ICON_HOME).c_str())) { m_cameraController->reset({0, 0, 0}); }
    ImGui::PopStyleVar();

    ImDrawList *draw_list = ImGui::GetForegroundDrawList();
    const IRect &selectionRect = m_rectSelection.rect;
    ImVec2 min = {
        viewportOrigin.x + selectionRect.origin.x, viewportOrigin.y + selectionRect.origin.y
    };
    ImVec2 max = {min.x + selectionRect.size.width, min.y + selectionRect.size.height};
    draw_list->AddRectFilled(min, max, IM_COL32(100, 100, 100, 100));

    ImGui::End();
    ImGui::PopStyleVar();
    bool mouseDown = Input::isMouseButtonPressed(Fern::MouseButton::LEFT);
    if (m_focused && !m_gizmos.isUsing()) {
        if (!m_rectSelection.isStarted && mouseDown) {
            bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
            bool shift =
                ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
            beginRectSelection(ctrl || shift);
        } else if (m_rectSelection.isStarted && mouseDown) {
            updateRectSelection();
        } else if (m_rectSelection.isStarted && !mouseDown) {
            endRectSelection();
        }
    } else if (m_rectSelection.isStarted && (!m_gizmos.isUsing() || !m_focused)) {
        endRectSelection();
    }
    readIdsMemoryIfNeed();
}

std::unordered_set<UUID>
ViewportPanel::getEntitiesFromRequest(const ViewportPanel::ReadFrameBufferRequest &request) {
    std::unordered_set<UUID> ids;
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    for (int regX = 0; regX < request.bufferSize.width; regX++) {
        for (int regY = 0; regY < request.bufferSize.height; regY++) {
            uint32_t texelIndex = request.bufferSize.width * regY + regX;
            uint32_t *buffer = static_cast<uint32_t *>(request.mem.data);
            uint32_t hoveredId = buffer[texelIndex];
            if (hoveredId != 0) { ids.insert(hoveredId); }
        }
    }
    return ids;
}

void ViewportPanel::readIdsMemoryIfNeed() {
    if (m_rectSelection.readRequests.empty()) { return; }
    std::unordered_set<UUID> ids;
    auto &requests = m_rectSelection.readRequests;
    auto it = requests.begin();
    bool hasRequests = false;
    while (it != requests.end()) {
        ReadFrameBufferRequest &request = *it;
        if (Miren::getFrameNumber() >= request.ready) {
            std::unordered_set<UUID> _ids = getEntitiesFromRequest(request);
            ids.insert(_ids.begin(), _ids.end());
            request.mem.release();
            it = requests.erase(it);
            hasRequests = true;
        } else {
            it++;
        }
    }
    if (!hasRequests) { return; }
    std::unordered_set<UUID> select;
    std::unordered_set<UUID> unselect;
    // Был зажат ctrl или shift
    if (m_rectSelection.appendSelection) {
        // Те которые есть в m_rectSelection.currentSelection и их нет в ids добавить в unselect.
        // То есть, они вышли из региона текущего выделения. Снимаем выделение
        std::unordered_set<UUID> currentIds = m_rectSelection.currentSelection;
        for (UUID currentlySelected : currentIds) {
            if (!ids.contains(currentlySelected) &&
                !m_rectSelection.initialSelection.contains(currentlySelected)) {
                unselect.insert(currentlySelected);
                m_rectSelection.currentSelection.erase(currentlySelected);
            }
        }
        // Те которые есть в m_rectSelection.initialSelection и их нет в ids добавить в select.
        for (UUID initiallySelected : m_rectSelection.initialSelection) {
            if (!ids.contains(initiallySelected) &&
                !m_rectSelection.currentSelection.contains(initiallySelected)) {
                select.insert(initiallySelected);
                m_rectSelection.currentSelection.insert(initiallySelected);
            }
        }
        // Выделить те, которые попали в регион
        for (UUID id : ids) {
            bool selectedBefore = m_rectSelection.initialSelection.contains(id);
            bool selectedCurrently = m_rectSelection.currentSelection.contains(id);
            // Если был выбран, применяем инверсию. Надо снять выделение
            if (selectedBefore && selectedCurrently) {
                unselect.insert(id);
                m_rectSelection.currentSelection.erase(id);
                // Если не был выбран, выбираем. Инверсия выделения в регионе
            } else if (!selectedBefore && !selectedCurrently) {
                select.insert(id);
                m_rectSelection.currentSelection.insert(id);
            }
        }
    } else {
        // Выделить те, которые попали в регион
        for (UUID id : ids) {
            if (!m_rectSelection.currentSelection.contains(id)) {
                select.insert(id);
                m_rectSelection.currentSelection.insert(id);
            }
        }
        std::unordered_set<UUID> currentIds = m_rectSelection.currentSelection;
        // Убрать те, которые не попали
        for (UUID id : currentIds) {
            if (!ids.contains(id)) {
                unselect.insert(id);
                m_rectSelection.currentSelection.erase(id);
            }
        }
    }
    SelectionContext::addSelectedEntities(select);
    SelectionContext::removeSelectedEntities(unselect);
}

void ViewportPanel::beginRectSelection(bool append) {
    m_rectSelection.isStarted = true;
    m_rectSelection.appendSelection = append;
    m_rectSelection.rect =
        IRect(Input::getMouseViewportPositionX(), Input::getMouseViewportPositionY(), 0, 0);
    m_rectSelection.currentSelection = SelectionContext::getSelectedEntities();
    m_rectSelection.initialSelection = m_rectSelection.currentSelection;
}

void ViewportPanel::updateRectSelection() {
    IRect selectedRegion = m_rectSelection.rect;
    selectedRegion.size.width = Input::getMouseViewportPositionX() - m_rectSelection.rect.origin.x;
    selectedRegion.size.height = Input::getMouseViewportPositionY() - m_rectSelection.rect.origin.y;
    if (selectedRegion.size.width == 0) { selectedRegion.size.width = 1; }
    if (selectedRegion.size.height == 0) { selectedRegion.size.height = 1; }
    if (m_rectSelection.isEqualTo(selectedRegion)) { return; }
    m_rectSelection.rect = selectedRegion;
    ReadFrameBufferRequest &request = m_rectSelection.readRequests.emplace_back();
    if (selectedRegion.size.width < 0) {
        selectedRegion.origin.x += selectedRegion.size.width;
        selectedRegion.size.width = -selectedRegion.size.width;
    }
    if (selectedRegion.size.height < 0) {
        selectedRegion.origin.y += selectedRegion.size.height;
        selectedRegion.size.height = -selectedRegion.size.height;
    }
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    selectedRegion.size.x *= dpi.x;
    selectedRegion.size.y *= dpi.y;
    selectedRegion.origin.x *= dpi.x;
    selectedRegion.origin.y *= dpi.y;
    request.bufferSize = selectedRegion.size;
    uint32_t bufferSize = sizeof(uint32_t) * selectedRegion.size.width * dpi.width *
                          selectedRegion.size.height * dpi.height;
    request.mem = Foundation::Memory::alloc(bufferSize);
    memset(request.mem.data, 0, bufferSize);
    uint32_t textureHeight = m_viewport.getFrame().size.height * dpi.y;
    request.ready = Miren::readFrameBuffer(
        m_viewport.getSceneFrameBuffer(),
        1,
        selectedRegion.origin.x,
        // Так как текстура хранится перевернутой, тут мы переворачиваем координату по Y
        textureHeight - selectedRegion.origin.y - selectedRegion.size.height,
        selectedRegion.size.width,
        selectedRegion.size.height,
        request.mem.data
    );
}

void ViewportPanel::endRectSelection() {
    m_rectSelection.isStarted = false;
    m_rectSelection.rect = IRect(0, 0, 0, 0);
}

void ViewportPanel::unselectAll() {
    World *currentWorld = GameContext::getCurrentWorld();
    if (!currentWorld) { return; }
    SelectionContext::unselectAll();
}

void ViewportPanel::setCamera(Camera *camera) {
    m_camera = camera;
    m_gizmos.setCamera(camera);
    if (m_camera) { m_camera->setViewportSize(m_viewport.getFrame().size); }
}

void ViewportPanel::setWorld(World *world) {
    m_gizmos.setWorld(world);
}

bool ViewportPanel::isFocused() {
    return m_focused;
}

bool ViewportPanel::isHovered() {
    return m_hovered;
}

void ViewportPanel::focus() {
    m_focusNextFrame = true;
}

Miren::ViewId ViewportPanel::getRenderingView() {
    return m_viewport.getRenderingView();
}

Miren::ViewId ViewportPanel::getSelectionRenderingView() {
    return m_viewport.getSelectionRenderingView();
}

} // namespace Panda
