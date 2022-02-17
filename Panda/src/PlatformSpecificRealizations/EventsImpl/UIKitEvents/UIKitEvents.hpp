//
// Created by Admin on 17.02.2022.
//

#pragma once

#include "Panda/Events/IEvents.hpp"

namespace Panda {

class UIKitEvents : public IEvents {
public:
    UIKitEvents() = default;
    ~UIKitEvents() override;
    void initialize() override;
    void pollEvents() override;
    void toggleCursorLock() override;
    void setFrameBufferSizeCallback(std::function<void(GSize)>) override;
    void addWindowSizeListener(WindowSizeListener *listener) override;
    void removeWindowSizeListener(WindowSizeListener *listener) override;
    bool isCursorLocked() override;
    bool isKeyPressed(Key key) override;
    bool isKeyJustPressed(Key key) override;
    float getDeltaX() override;
    float getDeltaY() override;
};

} // namespace Panda