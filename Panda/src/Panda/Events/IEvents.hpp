//
// Created by Admin on 05.02.2022.
//

#pragma once

#include "Key.hpp"
#include "WindowSizeListener.hpp"

namespace Panda {

class IWindow;

class IEvents {
public:
    virtual ~IEvents() = default;
    virtual void initialize() = 0;
    virtual void pollEvents() = 0;
    virtual void toggleCursorLock() = 0;
    virtual void setFrameBufferSizeCallback(std::function<void(GSize)>) = 0;
    virtual void addWindowSizeListener(WindowSizeListener *listener) = 0;
    virtual void removeWindowSizeListener(WindowSizeListener *listener) = 0;
    virtual bool isCursorLocked() = 0;
    virtual bool isKeyPressed(Key key) = 0;
    virtual bool isKeyJustPressed(Key key) = 0;
    virtual float getDeltaX() = 0;
    virtual float getDeltaY() = 0;
};

} // namespace Panda
