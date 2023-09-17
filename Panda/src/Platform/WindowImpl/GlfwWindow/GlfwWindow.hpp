//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"

#include <Foundation/Foundation.hpp>
#include <GLFW/glfw3.h>

namespace Panda {

class GlfwWindow : public Window {
public:
    GlfwWindow(const char *title, Size size, bool isFullscreen, bool isMaximized);
    ~GlfwWindow() override;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    bool isMaximized() override;
    void setMaximized(bool isMaximized) override;
    void pollEvents() override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void setCursor(Cursor cursor) override;
    void setEventQueue(EventQueue *eventQueue) override;
    Size getSize() override;
    Size getDpi() override;

private:
    void resetCursorPos();
    void addEventHandlers();
    bool m_isCursorLocked;
    bool m_isFullScreen;
    Size m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
    EventQueue *m_eventQueue;
    GLFWcursor *cursors[Cursor::COUNT];
};

} // namespace Panda
