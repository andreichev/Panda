//
// Created by Michael Andreichev on 29.11.2023.
//

#pragma once

#include "PandaUI/View.hpp"
#include "PandaUI/UIRenderer.hpp"
#include "PandaUI/UICamera.hpp"

namespace PandaUI {

class Context final {
public:
    void update(double deltaTime);
    void updateViewportSize(Size size);
    void updateViewId(Miren::ViewId viewId);
    Size getViewportSize() {
        return m_viewportSize;
    }
    template<typename T, typename... Args>
    Foundation::Shared<T> makeView(Args &&...args) {
        // TODO: Собственный аллокатор для PandaUI
        return Foundation::makeShared<T>(std::forward<Args>(args)...);
    }

    Foundation::Shared<View> getRootView() {
        return m_rootView;
    }

    void setRootView(Foundation::Shared<View> view) {
        m_rootView = view;
    }

    UIRenderer &getRenderer() {
        return m_renderer;
    }

    static Context &shared();
    static bool isInitialized() {
        return s_shared != nullptr;
    }

private:
    Context();

    static Context *s_shared;
    Foundation::Shared<View> m_rootView;
    UIRenderer m_renderer;
    UICamera m_camera;
    Miren::ViewId m_mirenViewId;
    Size m_viewportSize;

    friend class Layer;
};

} // namespace PandaUI
