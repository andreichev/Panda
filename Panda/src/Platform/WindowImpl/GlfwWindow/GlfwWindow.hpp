//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"
#include <GLFW/glfw3.h>
#include "Panda/Base/Base.hpp"
#include "Panda/Application/ApplicationContext.hpp"

namespace Panda {

class GlfwWindow : public Window {
public:
    GlfwWindow(const char *title, GSize size, bool isFullscreen);
    ~GlfwWindow() override;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    void pollEvents() override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    ApplicationContext *context;

private:
    void resetCursorPos();
    void addEventHandlers();
    bool cursorLocked;
    bool m_isFullScreen;
    GSize m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
};

} // namespace Panda
