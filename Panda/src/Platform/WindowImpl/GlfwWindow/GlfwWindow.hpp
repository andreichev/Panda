//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"
#include "Panda/Base/Base.hpp"

#include <GLFW/glfw3.h>

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
    void setEventQueue(EventQueue *eventQueue) override;
    GSize getSize() override;

private:
    void resetCursorPos();
    void addEventHandlers();
    bool m_isCursorLocked;
    bool m_isFullScreen;
    GSize m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
    EventQueue *m_eventQueue;
};

} // namespace Panda
