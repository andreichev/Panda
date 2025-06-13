#include "ViewportPanel.hpp"

#include <Panda.hpp>
#include <imgui.h>
#include <Panda/ImGui/FontAwesome.h>

namespace Panda {

ViewportPanel::ViewportPanel(ViewportPanelOutput *output, CameraController *cameraController)
    : m_output(output)
    , m_viewport()
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

void ViewportPanel::viewportDrawOutline() {
    m_viewport.viewportDrawOutline();
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
    const Rect &selectionRect = m_rectSelection.rect;
    ImVec2 min = {
        viewportOrigin.x + selectionRect.origin.x, viewportOrigin.y + selectionRect.origin.y
    };
    ImVec2 max = {min.x + selectionRect.size.width, min.y + selectionRect.size.height};
    draw_list->AddRectFilled(min, max, IM_COL32(100, 100, 100, 100));

    ImGui::End();
    ImGui::PopStyleVar();
    m_viewport.viewportReadIdsBuffer();
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
}

void ViewportPanel::beginRectSelection(bool append) {
    m_rectSelection.isStarted = true;
    m_rectSelection.appendSelection = append;
    m_rectSelection.rect =
        Rect(Input::getMouseViewportPositionX(), Input::getMouseViewportPositionY(), 0, 0);
    m_rectSelection.currentSelection = m_output->viewportGetSelectedIds();
    m_rectSelection.initialSelection = m_rectSelection.currentSelection;
}

void ViewportPanel::updateRectSelection() {
    m_rectSelection.rect.size.width =
        Input::getMouseViewportPositionX() - m_rectSelection.rect.origin.x;
    m_rectSelection.rect.size.height =
        Input::getMouseViewportPositionY() - m_rectSelection.rect.origin.y;
    if (m_rectSelection.rect.size.isZero()) { return; }
    std::unordered_set<UUID> ids = m_viewport.getEntitiesInsideRect(m_rectSelection.rect);
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
    m_output->viewportPickEntitiesWithId(select);
    m_output->viewportUnselectEntitiesWithId(unselect);
}

void ViewportPanel::endRectSelection() {
    // Если был только клик, не растягивали прямоугольник выделения
    if (m_rectSelection.rect.size.isZero()) {
        float mouseX = Input::getMouseViewportPositionX();
        float mouseY = Input::getMouseViewportPositionY();
        uint32_t hoveredId = m_viewport.getEntityInsidePoint({mouseX, mouseY});
        if (!m_rectSelection.appendSelection) { m_output->viewportUnselectAll(); }
        if (hoveredId) {
            bool alreadySelected = m_rectSelection.currentSelection.contains(hoveredId);
            if (alreadySelected) {
                m_output->viewportUnselectEntitiesWithId({hoveredId});
            } else {
                m_output->viewportPickEntitiesWithId({hoveredId});
            }
        }
    }
    m_rectSelection.isStarted = false;
    m_rectSelection.rect = Rect(0, 0, 0, 0);
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

Miren::ViewId ViewportPanel::getMirenView() {
    return m_viewport.getRenderingView();
}

} // namespace Panda
