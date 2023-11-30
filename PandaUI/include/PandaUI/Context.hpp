//
// Created by Michael Andreichev on 29.11.2023.
//

#pragma once

#include "PandaUI/View.hpp"
#include "PandaUI/UICamera.hpp"

#include <Panda/Renderer/Renderer2D.hpp>
#include <Panda/GameLogic/Components/CameraComponent.hpp>

namespace PandaUI {

class Context final {
public:
    Context();
    void update(double deltaTime);
    void updateViewportSize(Size size);
    void updateViewId(Miren::ViewId viewId);

    static Context &shared();
    static bool isInitialized() {
        return s_shared != nullptr;
    }

private:
    static Context *s_shared;
    std::vector<View> m_views;
    Panda::Renderer2D m_renderer2d;
    UICamera m_camera;
    Miren::ViewId m_viewId;
    Size m_viewportSize;

    friend class Layer;
};

} // namespace PandaUI
