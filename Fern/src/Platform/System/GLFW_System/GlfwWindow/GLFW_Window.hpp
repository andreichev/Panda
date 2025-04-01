//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Fern/Window/Window.hpp"

#include <Foundation/Foundation.hpp>
#include <GLFW/glfw3.h>

namespace Fern {

class GLFW_Window : public Window {
public:
    GLFW_Window(
        const char *title,
        Fern::Rect rect,
        Fern::WindowState state,
        Fern::DrawingContextType contextType
    );
    ~GLFW_Window() override;
    void setTitle(const char *title) override;
    WindowState getState() override;
    void setState(WindowState state) override;
    void setModifiedState(bool isUnsavedChanges) override;
    bool getModifiedState() override;
    void setResizable(bool isResizable) override;
    void setSize(Size size) override;
    Size getSize() override;
    Size getDpi() override;
    Vec2 getCursorPosition() override;
    bool setProgressBar(float progress) override;
    bool endProgressBar() override;
    // Graphics context
    GraphicsContext *getDrawingContext() override;

    void ignoreNextMouseEvents();
    GLFWwindow *getHandle();
    bool isFocused();

private:
    void resetCursorPos();
    void addEventHandlers();

    bool m_isCursorLocked;
    Size m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
    Fern::GraphicsContext *m_graphicsContext;
    int m_ignoreMouseEvents;
};

} // namespace Fern
