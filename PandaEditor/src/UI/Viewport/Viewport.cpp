#include "Viewport.hpp"

#include <Panda.hpp>
#include <PandaUI/PandaUI.hpp>
#include <imgui.h>
#include <Panda/ImGui/FontAwesome.h>

namespace Panda {

Viewport::Viewport(ViewportOutput *output, CameraController *cameraController)
    : m_output(output)
    , m_frame()
    , m_rectSelection()
    , m_idsBuffer()
    , m_camera(nullptr)
    , m_cameraController(cameraController)
    , m_focused(false)
    , m_hovered(false)
    , m_focusNextFrame(true)
    , m_sceneFB()
    , m_sceneFbSpecification()
    , m_sceneView(1)
    , m_colorAttachment()
    , m_gizmos(nullptr, cameraController) {}

void Viewport::initWithSize(Vec2 size) {
    m_frame.size = size;
    PandaUI::initialize();
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    Fern::Size windowSize = Application::get()->getMainWindow()->getSize();
    Miren::TextureCreate create;
    create.m_data = Foundation::Memory(nullptr);
    create.m_format = Miren::TextureFormat::RGBA8;
    create.m_width = m_frame.size.width * dpi.width;
    create.m_height = m_frame.size.height * dpi.height;
    m_colorAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::R32UI;
    Miren::TextureHandle idAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::DEPTH24STENCIL8;
    Miren::TextureHandle depthAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment attachments[] = {m_colorAttachment, idAttachment, depthAttachment};
    m_sceneFbSpecification = Miren::FrameBufferSpecification(attachments, 3);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewport(
        m_sceneView,
        Miren::Rect(0, 0, m_frame.size.width * dpi.width, m_frame.size.height * dpi.height)
    );
    Miren::setViewport(
        0, Miren::Rect(0, 0, windowSize.width * dpi.width, windowSize.height * dpi.height)
    );
    Miren::setViewFrameBuffer(m_sceneView, m_sceneFB);
    Miren::setViewClear(m_sceneView, 0x000000cff);
    Miren::setViewClearAttachments(m_sceneView, {Miren::Clear(1, 0)});
    PandaUI::Context::shared().updateViewId(m_sceneView);
    uint32_t bufferSize =
        sizeof(uint32_t) * m_frame.size.width * dpi.width * m_frame.size.height * dpi.height;
    m_idsBuffer = Foundation::Memory::alloc(bufferSize);
    memset(m_idsBuffer.data, 0, bufferSize);
}

void Viewport::updateViewportSize(Size size) {
    if (size.width < 1 || size.height < 1) { return; }
    m_frame.size = size;
    PandaUI::Context::shared().updateViewportSize({size.width, size.height});
    if (m_camera) { m_camera->setViewportSize(size); }
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    Miren::setViewport(
        m_sceneView, Miren::Rect(0, 0, size.width * dpi.width, size.height * dpi.height)
    );
    // COLOR ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[0].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    // ID ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[1].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    // DEPTH ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[2].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    Miren::deleteFrameBuffer(m_sceneFB);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewFrameBuffer(m_sceneView, m_sceneFB);
    m_idsBuffer.release();
    uint32_t bufferSize =
        sizeof(uint32_t) * m_frame.size.width * dpi.width * m_frame.size.height * dpi.height;
    m_idsBuffer = Foundation::Memory::alloc(bufferSize);
    memset(m_idsBuffer.data, 0, bufferSize);
}

void Viewport::onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen) {
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
    if (m_frame.size != viewportSpace) { updateViewportSize(viewportSpace); }
    m_hovered = ImGui::IsWindowHovered();
    if ((m_hovered && Input::isMouseButtonPressed(Fern::MouseButton::RIGHT)) || m_focusNextFrame) {
        ImGui::SetWindowFocus();
        m_focusNextFrame = false;
    }
    m_focused = ImGui::IsWindowFocused();
    Application::get()->getImGuiLayer()->setBlockEvents(!m_hovered);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);
    m_frame.origin = ImGui::GetCursorScreenPos();
    Input::setViewportFrame(m_frame);
    ImGui::Image((void *)(uintptr_t)m_colorAttachment.id, m_frame.size, ImVec2(0, 1), ImVec2(1, 0));
    m_gizmos.onImGuiRender(sceneState, m_frame);

    ImGuiStyle &style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::SetCursorPos({10, 40});
    if (ImGui::Button(getString(ICON_HOME).c_str())) { m_cameraController->reset({0, 0, 0}); }
    ImGui::PopStyleVar();

    ImDrawList *draw_list = ImGui::GetForegroundDrawList();
    const Rect &selectionRect = m_rectSelection.rect;
    ImVec2 min = {
        m_frame.origin.x + selectionRect.origin.x, m_frame.origin.y + selectionRect.origin.y
    };
    ImVec2 max = {min.x + selectionRect.size.width, min.y + selectionRect.size.height};
    draw_list->AddRectFilled(min, max, IM_COL32(100, 100, 100, 100));

    ImGui::End();
    ImGui::PopStyleVar();

    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    Miren::readFrameBuffer(
        m_sceneFB,
        1,
        0,
        0,
        m_frame.size.width * dpi.x,
        m_frame.size.height * dpi.y,
        m_idsBuffer.data
    );
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

void Viewport::beginRectSelection(bool append) {
    m_rectSelection.isStarted = true;
    m_rectSelection.appendSelection = append;
    m_rectSelection.rect =
        Rect(Input::getMouseViewportPositionX(), Input::getMouseViewportPositionY(), 0, 0);
    m_rectSelection.currentSelection = m_output->viewportGetSelectedIds();
    m_rectSelection.initialSelection = m_rectSelection.currentSelection;
}

void Viewport::updateRectSelection() {
    m_rectSelection.rect.size.width =
        Input::getMouseViewportPositionX() - m_rectSelection.rect.origin.x;
    m_rectSelection.rect.size.height =
        Input::getMouseViewportPositionY() - m_rectSelection.rect.origin.y;
    if (m_rectSelection.rect.size.isZero()) { return; }
    std::unordered_set<UUID> ids = getEntitiesInsideRectSelection();
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

void Viewport::endRectSelection() {
    // Если был только клик, не растягивали прямоугольник выделения
    if (m_rectSelection.rect.size.isZero()) {
        Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
        int mouseX = Input::getMouseViewportPositionX() * dpi.x;
        int mouseY = m_frame.size.height * dpi.y - Input::getMouseViewportPositionY() * dpi.y;
        uint32_t texelIndex = m_frame.size.width * dpi.x * mouseY + mouseX;
        uint32_t *buffer = static_cast<uint32_t *>(m_idsBuffer.data);
        uint32_t hoveredId = buffer[texelIndex];
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

std::unordered_set<UUID> Viewport::getEntitiesInsideRectSelection() {
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    //      m_frame
    //   _________________________
    //   |      selectedRegion   |
    //   |      ___________      |
    //   |     |           |     |
    //   |     |___________|     |
    //   |_______________________|
    //
    Rect selectedRegion = m_rectSelection.rect;
    selectedRegion.size.x *= dpi.x;
    selectedRegion.size.y *= dpi.y;
    selectedRegion.origin.x *= dpi.x;
    selectedRegion.origin.y *= dpi.y;
    std::unordered_set<UUID> ids;
    if (selectedRegion.size.width < 0) {
        selectedRegion.origin.x += selectedRegion.size.width;
        selectedRegion.size.width = -selectedRegion.size.width;
    }
    if (selectedRegion.size.height < 0) {
        selectedRegion.origin.y += selectedRegion.size.height;
        selectedRegion.size.height = -selectedRegion.size.height;
    }
    for (int regX = 0; regX < selectedRegion.size.width; regX++) {
        for (int regY = 0; regY < selectedRegion.size.height; regY++) {
            int texelX = selectedRegion.origin.x + regX;
            int texelY = m_frame.size.height * dpi.y - selectedRegion.origin.y - regY;
            uint32_t texelIndex = m_frame.size.width * dpi.x * texelY + texelX;
            uint32_t *buffer = static_cast<uint32_t *>(m_idsBuffer.data);
            uint32_t hoveredId = buffer[texelIndex];
            if (hoveredId != 0) { ids.insert(hoveredId); }
        }
    }
    return ids;
}

void Viewport::setCamera(Camera *camera) {
    m_camera = camera;
    m_gizmos.setCamera(camera);
    if (m_camera) { m_camera->setViewportSize(m_frame.size); }
}

void Viewport::setWorld(World *world) {
    m_gizmos.setWorld(world);
}

bool Viewport::isFocused() {
    return m_focused;
}

bool Viewport::isHovered() {
    return m_hovered;
}

void Viewport::focus() {
    m_focusNextFrame = true;
}

Miren::ViewId Viewport::getMirenView() {
    return m_sceneView;
}

} // namespace Panda
