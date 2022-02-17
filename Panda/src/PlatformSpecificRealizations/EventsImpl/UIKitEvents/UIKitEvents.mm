//
// Created by Admin on 17.02.2022.
//

#include "UIKitEvents.hpp"

namespace Panda {
    UIKitEvents::~UIKitEvents() {}

    void UIKitEvents::initialize() {}

    void UIKitEvents::pollEvents() {}

    void UIKitEvents::toggleCursorLock() {}

    void UIKitEvents::setFrameBufferSizeCallback(std::function<void(GSize)>) {}

    void UIKitEvents::addWindowSizeListener(WindowSizeListener *listener) {}

    void UIKitEvents::removeWindowSizeListener(WindowSizeListener *listener) {}

    bool UIKitEvents::isCursorLocked() { return false; }

    bool UIKitEvents::isKeyPressed(Key key) { return false; }

    bool UIKitEvents::isKeyJustPressed(Key key) { return false; }

    float UIKitEvents::getDeltaX() { return 0.f; }

    float UIKitEvents::getDeltaY() { return 0.f; }
}