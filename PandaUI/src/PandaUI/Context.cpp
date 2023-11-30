//
// Created by Michael Andreichev on 29.11.2023.
//

#include "PandaUI/Context.hpp"

#include <Foundation/Foundation.hpp>

namespace PandaUI {

Context *Context::s_shared = nullptr;

Context &Context::shared() {
    PND_ASSERT(s_shared != nullptr, "UICONTEXT IS NOT INITIALIZED");
    return *s_shared;
}

Context::Context()
    : m_viewId(0)
    , m_viewportSize(100, 100) {
    m_camera.updateViewportSize(m_viewportSize);
    m_renderer2d.setCamera(&m_camera);
    m_renderer2d.setViewId(0);

    Rect rect(30, 30, 50, 200);
    View view(rect);
    view.setBackgroundColor({0x66FF66FF});
    m_views.push_back(view);
}

void Context::update(double deltaTime) {
    m_renderer2d.begin();
    for (View &view : m_views) {
        Panda::Renderer2D::RectData rect;
        Point origin = view.getFrame().origin;
        Size size = view.getFrame().size;
        rect.center = Panda::Vec3(origin.x + size.width / 2, origin.y + size.height / 2, 0.f);
        rect.size = Panda::Size(size.width, size.height);
        Color color = view.getBackgroundColor();
        rect.color = {color.r, color.g, color.b, color.a};
        m_renderer2d.drawRect(rect);
    }
    m_renderer2d.end();
}

void Context::updateViewportSize(Size size) {
    m_viewportSize = size;
    m_camera.updateViewportSize(size);
    for (View &view : m_views) {
        view.layout();
    }
}
void Context::updateViewId(Miren::ViewId viewId) {
    m_viewId = viewId;
    m_renderer2d.setViewId(viewId);
}

} // namespace PandaUI