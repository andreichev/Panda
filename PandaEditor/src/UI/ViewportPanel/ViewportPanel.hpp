//
// Created by Michael Andreichev on 14.09.2023.
//

#pragma once

#include "UI/Gizmos/Gizmos.hpp"
#include "Camera/CameraController.hpp"

#include <Viewport/Viewport.hpp>
#include <Panda/GameLogic/Camera.hpp>
#include <Miren/Miren.hpp>

namespace Panda {

class ViewportPanel final {
public:
    struct ReadFrameBufferRequest {
        int ready;
        Foundation::Memory mem;
        // Multiplied by screen content scale
        ISize bufferSize;
    };

    struct RectSelection {
        bool isStarted = false;
        bool appendSelection = false;
        IRect rect;
        std::unordered_set<UUID> initialSelection;
        std::unordered_set<UUID> currentSelection;
        std::vector<ReadFrameBufferRequest> readRequests;

        bool isEqualTo(IRect rect);
    };

    ViewportPanel(CameraController *cameraController);
    ~ViewportPanel();

    void initWithSize(Vec2 size);
    void setCamera(Camera *camera);
    void setWorld(World *world);
    void onImGuiRender(SceneState sceneState, float offsetY, bool fullScreen);
    void drawOutline(float dt);
    void focus();
    bool isFocused();
    bool isHovered();

private:
    void updateViewportSize(Size size);
    std::unordered_set<UUID> getEntitiesFromRequest(const ReadFrameBufferRequest &request);
    void readIdsMemoryIfNeed();
    void beginRectSelection(bool appendSelection);
    void updateRectSelection();
    void endRectSelection();
    void unselectAll();

    Viewport m_viewport;
    Gizmos m_gizmos;
    Camera *m_camera;
    CameraController *m_cameraController;
    bool m_focusNextFrame;
    bool m_focused;
    bool m_hovered;
    RectSelection m_rectSelection;
};

} // namespace Panda
