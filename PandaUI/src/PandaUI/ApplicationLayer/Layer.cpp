//
// Created by Michael Andreichev on 28.10.2023.
//

#include "PandaUI/ApplicationLayer/Layer.hpp"

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

void Layer::onEvent(Panda::Event *event) {
    // switch (event->type) {
    //     case Panda::EventType::TouchBegan:
    //         Panda::TouchBegan
    //
    // }
}

} // namespace PandaUI
