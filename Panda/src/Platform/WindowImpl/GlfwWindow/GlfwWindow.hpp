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
    ~GlfwWindow() override;
    void initialize(const char *title, GSize size, bool isFullscreen) override;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    GSize getWindowSize() override;
    void pollEvents() override;
    ApplicationContext* context;
private:
    void addEventHandlers();
    bool m_isFullScreen;
    GSize m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
};

} // namespace Panda
