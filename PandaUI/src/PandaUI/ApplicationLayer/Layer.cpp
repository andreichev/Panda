//
// Created by Michael Andreichev on 28.10.2023.
//

#include "PandaUI/ApplicationLayer/Layer.hpp"
#include "PandaUI/Config.hpp"

#include <Fern/Events/WindowEvents.hpp>

namespace PandaUI {

Layer::Layer() {
    Context::s_shared = &m_context;
}

Layer::~Layer() {
    Context::s_shared = nullptr;
}

void Layer::onAttach() {}

void Layer::onDetach() {}

void Layer::onUpdate(double deltaTime) {
    m_context.update(deltaTime);
}

void Layer::onImGuiRender() {}

void Layer::onEvent(Fern::Event *event) {
    using namespace Panda;
    switch (event->type) {
        case Fern::EventType::WindowResize: {
#ifdef AUTO_RESIZE_ROOT
            const Fern::WindowResizeEvent *ev = static_cast<const Fern::WindowResizeEvent *>(event);
            m_context.updateViewportSize(Size(ev->getWidth(), ev->getHeight()));
#endif
            break;
        }
        default:
            break;
    }
}

} // namespace PandaUI
