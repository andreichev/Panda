//
// Created by Michael Andreichev on 28.10.2023.
//

#include "PandaUI/ApplicationLayer/PandaAppUILayer.hpp"

namespace PandaUI {

PandaAppUILayer::PandaAppUILayer() {}

void PandaAppUILayer::onAttach() {}

void PandaAppUILayer::onDetach() {}

void PandaAppUILayer::onUpdate(double deltaTime) {
    m_renderer2d.begin();
    for (UIView &view : m_views) {
        Panda::Renderer2D::RectData rect;
        Point origin = view.getFrame().origin;
        rect.origin = Panda::Vec3(origin.x, origin.y, 0.f);
        Size size = view.getFrame().size;
        rect.size = Panda::Size(size.width, size.height);
        m_renderer2d.drawRect(rect);
    }
    m_renderer2d.end();
}

void PandaAppUILayer::onImGuiRender() {}

void PandaAppUILayer::onEvent(Panda::Event *event) {
    // switch (event->type) {
    //     case Panda::EventType::TouchBegan:
    //         Panda::TouchBegan
    //
    // }
}

} // namespace PandaUI
