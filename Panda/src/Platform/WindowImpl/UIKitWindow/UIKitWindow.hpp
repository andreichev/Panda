//
// Created by Admin on 17.02.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"

namespace Panda {

class UIKitWindow : public IWindow {
public:
    UIKitWindow() = default;
    ~UIKitWindow() override;
    void initialize(const char *title, GSize size, bool isFullscreen) override;
    double getTime() override;
    bool isShouldClose() override;
    void setShouldClose(bool flag) override;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    GSize getWindowSize() override;
    void swapBuffers() override;
    void *getNativeWindow() override;
    void *getProcAddressFuncPointer() override;
};

} // namespace Panda
