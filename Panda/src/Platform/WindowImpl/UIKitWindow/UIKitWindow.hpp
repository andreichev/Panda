//
// Created by Admin on 17.02.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"

namespace Panda {

class UIKitWindow : public Window {
public:
    UIKitWindow();
    ~UIKitWindow() override = default;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    void pollEvents() override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void setEventQueue(EventQueue *eventQueue) override;
    GSize getSize() override;

private:
    GSize size;
};

} // namespace Panda
