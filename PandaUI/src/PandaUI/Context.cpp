//
// Created by Michael Andreichev on 29.11.2023.
//

#include "PandaUI/Context.hpp"

#include <Foundation/Foundation.hpp>
#include <Panda/Application/Application.hpp>
#include <yoga/Yoga.h>

namespace PandaUI {

Context *Context::s_shared = nullptr;

Context &Context::shared() {
    PND_ASSERT(s_shared != nullptr, "UICONTEXT IS NOT INITIALIZED");
    return *s_shared;
}

Context::Context()
    : m_mirenViewId(0)
    , m_viewportSize()
    , m_rootView(nullptr) {
    Fern::Window *window = Panda::Application::get()->getMainWindow();
    m_viewportSize = Size(window->getSize().width, window->getSize().height);
    m_camera.setViewportSize(m_viewportSize);
    // View matrix is identity
    m_renderer2d.setViewProj(m_camera.getProjection());
    // Make background view transparent
    if (m_rootView) { m_rootView->setBackgroundColor(Color(0x00000000)); }
    // YGConfigRef config = YGConfigNew();
    // YGConfigSetUseWebDefaults(config, false);
}

void Context::update(double deltaTime) {
    if (m_rootView) {
        m_renderer2d.begin(Panda::Renderer2D::Mode::DEFAULT, m_mirenViewId);
        m_rootView->render(0, 0);
        m_renderer2d.end();
    }
}

void Context::updateViewportSize(Size size) {
    if (m_rootView) {
        m_viewportSize = size;
        m_camera.setViewportSize(size);
        // View matrix is identity
        m_renderer2d.setViewProj(m_camera.getProjection());

        // m_rootView->styleSetAbsolute();
        // m_rootView->styleSetOrigin({0, 0});
        // LOG_INFO("SIZE: %d, %d", size.width, size.height);
        m_rootView->styleSetSize(size);
        m_rootView->calculateLayout();
    }
}

void Context::updateViewId(Miren::ViewId viewId) {
    m_mirenViewId = viewId;
}

} // namespace PandaUI