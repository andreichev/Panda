//
// Created by Admin on 17.02.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"

namespace Panda {

class UIKitWindow : public Window {
public:
    UIKitWindow() = default;
    ~UIKitWindow() override = default;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    void pollEvents() override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
};

} // namespace Panda
