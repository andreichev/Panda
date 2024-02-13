//
// Created by Michael Andreichev on 29.11.2023.
//

#pragma once

#include "PandaUI/View.hpp"
#include "PandaUI/UICamera.hpp"

#include <Panda/Renderer/Renderer2D.hpp>

namespace PandaUI {

class Context final {
public:
    void update(double deltaTime);
    void updateViewportSize(Size size);
    void updateViewId(Miren::ViewId viewId);

    template<typename T, typename... Args>
    T *makeView(Args &&...args) {
        // TODO: Собственный аллокатор для PandaUI
        return NEW(Foundation::getAllocator(), T)(std::forward<Args>(args)...);
    }

    void removeView(View *view) {
        DELETE(Foundation::getAllocator(), view);
    }

    View &getView() {
        return m_view;
    }

    Panda::Renderer2D &getRenderer() {
        return m_renderer2d;
    }

    static Context &shared();
    static bool isInitialized() {
        return s_shared != nullptr;
    }

private:
    Context();

    static Context *s_shared;
    View m_view;
    Panda::Renderer2D m_renderer2d;
    UICamera m_camera;
    Miren::ViewId m_mirenViewId;
    Size m_viewportSize;

    friend class Layer;
};

} // namespace PandaUI
