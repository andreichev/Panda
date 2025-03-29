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
    void setTitle(const char *title);
    WindowState getState();
    void setState(WindowState state);
    void setModifiedState(bool isUnsavedChanges);
    bool getModifiedState();
    void setResizable(bool isResizable);
    void setSize(Size size);
    Size getSize();
    Size getDpi();
    Vec2 getCursorPosition();
    bool setProgressBar(float progress);
    bool endProgressBar();
    bool isFocused();
    // Graphics context
    GraphicsContext *getDrawingContext();

    GLFWwindow *getHandle();

private:
    void resetCursorPos();
    void addEventHandlers();
    bool m_isCursorLocked;
    Size m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
    Fern::GraphicsContext *m_graphicsContext;
};

} // namespace Fern
