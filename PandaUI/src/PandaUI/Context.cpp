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
    : m_mirenViewId(0)
    , m_viewportSize(100, 100) {
    m_camera.updateViewportSize(m_viewportSize);
    m_renderer2d.setCamera(&m_camera);
    m_renderer2d.setViewId(0);
    // Make background view transparent
    m_view.setBackgroundColor(Color(0x00000000));

    Rect rect1(20, 20, 130, 30);
    View *view1 = makeView<View>(rect1);
    view1->setBackgroundColor({0xFFFFFFFF});
    getView().addSubview(view1);

    Rect rect2(20, 55, 130, 30);
    View *view2 = makeView<View>(rect2);
    view2->setBackgroundColor({0x1C3578FF});
    getView().addSubview(view2);

    Rect rect3(20, 90, 130, 30);
    View *view3 = makeView<View>(rect3);
    view3->setBackgroundColor({0xE4181CFF});
    getView().addSubview(view3);
}

void Context::update(double deltaTime) {
    m_renderer2d.begin();
    m_view.render();
    m_renderer2d.end();
}

void Context::updateViewportSize(Size size) {
    m_viewportSize = size;
    m_camera.updateViewportSize(size);
    m_view.setFrame(PandaUI::Rect(0, 0, size.width, size.height));
}

void Context::updateViewId(Miren::ViewId viewId) {
    m_mirenViewId = viewId;
    m_renderer2d.setViewId(viewId);
}

} // namespace PandaUI