//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Panda/Window/IWindow.hpp"
#include <GLFW/glfw3.h>
#include "Panda/Base/Base.hpp"

namespace Panda {

class GlfwWindow : public IWindow {
public:
    ~GlfwWindow() override;

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

private:
    bool m_isFullScreen;
    GSize m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
};

} // namespace Panda
