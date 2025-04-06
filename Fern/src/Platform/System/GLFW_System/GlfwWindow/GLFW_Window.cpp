//
// Created by Admin on 26.01.2022.
//

#include "GraphicsContext/GLFW_GLContext.hpp"
#include "GLFW_Window.hpp"
#include "Fern/Events/EventQueue.hpp"

#include <Foundation/PlatformDetection.hpp>

namespace Fern {

EventQueue *getEventQueue();

GLFW_Window::~GLFW_Window() {
    glfwDestroyWindow(m_windowHandle);
    glfwTerminate();
    F_DELETE(Foundation::getAllocator(), m_graphicsContext);
}

GLFW_Window::GLFW_Window(
    const char *title,
    Fern::Rect rect,
    Fern::WindowState state,
    Fern::DrawingContextType contextType
) {
    m_windowSizeBackup = rect.size;
    m_isCursorLocked = false;
    m_ignoreMouseEvents = 0;
    glfwDefaultWindowHints();
#ifdef PLATFORM_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(
        GLFW_MAXIMIZED, state == WindowState::WindowStateMaximized ? GLFW_TRUE : GLFW_FALSE
    );
    if (state == WindowState::WindowStateFullScreen) {
        GLFWmonitor *monitorHandle = glfwGetPrimaryMonitor();
        const GLFWvidmode *vidmode = glfwGetVideoMode(monitorHandle);
        m_windowSizeBackup = {(float)vidmode->width, (float)vidmode->height};
        m_windowHandle =
            glfwCreateWindow(vidmode->width, vidmode->height, title, monitorHandle, nullptr);
    } else {
        m_windowHandle =
            glfwCreateWindow((int)rect.size.width, (int)rect.size.height, title, nullptr, nullptr);
    }
    if (m_windowHandle == nullptr) {
        LOG_CRITICAL("Failed to create the GLFW window");
        exit(1);
    }
    glfwShowWindow(m_windowHandle);
    glfwFocusWindow(m_windowHandle);
    m_graphicsContext = F_NEW(Foundation::getAllocator(), GLFW_GLContext)(m_windowHandle);
    addEventHandlers();
}

void GLFW_Window::setTitle(const char *title) {
    glfwSetWindowTitle(m_windowHandle, title);
}

WindowState GLFW_Window::getState() {
    GLFWmonitor *monitorHandle = glfwGetPrimaryMonitor();
    if (glfwGetWindowMonitor(m_windowHandle) == monitorHandle) {
        return WindowState::WindowStateFullScreen;
    }
    if (glfwGetWindowAttrib(m_windowHandle, GLFW_ICONIFIED) == GLFW_TRUE) {
        return WindowState::WindowStateMinimized;
    }
    if (glfwGetWindowAttrib(m_windowHandle, GLFW_MAXIMIZED) == GLFW_TRUE) {
        return WindowState::WindowStateMaximized;
    }
    return WindowState::WindowStateNormal;
}

void GLFW_Window::setState(WindowState state) {
    GLFWmonitor *monitorHandle = glfwGetPrimaryMonitor();
    bool isFullScreen = glfwGetWindowMonitor(m_windowHandle) == monitorHandle;
    // Fullscreen handling
    {
        if (state == WindowState::WindowStateFullScreen) {
            const GLFWvidmode *vidmode = glfwGetVideoMode(monitorHandle);
            m_windowSizeBackup = getSize();
            glfwSetWindowMonitor(
                m_windowHandle, monitorHandle, 0, 0, vidmode->width, vidmode->height, GLFW_DONT_CARE
            );
        } else {
            glfwSetWindowMonitor(
                m_windowHandle,
                nullptr,
                0,
                0,
                m_windowSizeBackup.width,
                m_windowSizeBackup.height,
                GLFW_DONT_CARE
            );
            // Center the window
            const GLFWvidmode *vidmode = glfwGetVideoMode(monitorHandle);
            glfwSetWindowPos(
                m_windowHandle,
                (vidmode->width - m_windowSizeBackup.width) / 2,
                (vidmode->height - m_windowSizeBackup.height) / 2
            );
        }
    }
    // Other states handling
    {
        if (state == WindowState::WindowStateMinimized) {
            glfwIconifyWindow(m_windowHandle);
        } else if (state == WindowState::WindowStateMaximized) {
            glfwMaximizeWindow(m_windowHandle);
        } else if (state == WindowState::WindowStateNormal &&
                   glfwGetWindowAttrib(m_windowHandle, GLFW_ICONIFIED) == GLFW_TRUE) {
            glfwRestoreWindow(m_windowHandle);
        }
    }
    ignoreNextMouseEvents();
}

void GLFW_Window::setModifiedState(bool isUnsavedChanges) {
    // NOT SUPPORTED IN GLFW
}

bool GLFW_Window::getModifiedState() {
    // NOT SUPPORTED IN GLFW
    return false;
}

void GLFW_Window::setResizable(bool isResizable) {
    glfwSetWindowAttrib(m_windowHandle, GLFW_RESIZABLE, isResizable);
}

void GLFW_Window::setSize(Size size) {
    int count;
    GLFWmonitor **monitorsHandle = glfwGetMonitors(&count);
    if (count > 1) { return; }
    const GLFWvidmode *vidmode = glfwGetVideoMode(monitorsHandle[0]);
    glfwSetWindowSize(m_windowHandle, size.width, size.height);
    glfwSetWindowPos(
        m_windowHandle, (vidmode->width - size.width) / 2, (vidmode->height - size.height) / 2
    );
}

Size GLFW_Window::getSize() {
    int width, height;
    glfwGetWindowSize(m_windowHandle, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

Size GLFW_Window::getDpi() {
    float xscale, yscale;
    GLFWmonitor *monitor = glfwGetWindowMonitor(m_windowHandle);
    if (monitor) {
        glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    } else {
        int fbw, fbh;
        glfwGetFramebufferSize(m_windowHandle, &fbw, &fbh);
        int width, height;
        glfwGetWindowSize(m_windowHandle, &width, &height);
        xscale = width / fbw;
        yscale = height / fbh;
    }
    return Size(xscale, yscale);
}

Vec2 GLFW_Window::getCursorPosition() {
    double x, y;
    glfwGetCursorPos(m_windowHandle, &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

bool GLFW_Window::setProgressBar(float progress) {
    // NOT SUPPORTED IN GLFW
    return false;
}

bool GLFW_Window::endProgressBar() {
    return false;
}

bool GLFW_Window::isFocused() {
    return glfwGetWindowAttrib(m_windowHandle, GLFW_FOCUSED) == GLFW_TRUE;
}

// Graphics context
GraphicsContext *GLFW_Window::getDrawingContext() {
    return m_graphicsContext;
}

GLFWwindow *GLFW_Window::getHandle() {
    return m_windowHandle;
}

void GLFW_Window::addEventHandlers() {
    glfwSetWindowUserPointer(m_windowHandle, this);
    glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow *windowHandle, int width, int height) {
        GLFW_Window *self = static_cast<GLFW_Window *>(glfwGetWindowUserPointer(windowHandle));
        getEventQueue()->postSizeEvent(width, height);
    });
    glfwSetKeyCallback(
        m_windowHandle,
        [](GLFWwindow *windowHandle, int key, int scancode, int action, int mods) {
            getEventQueue()->postKeyEvent(
                static_cast<Key>(key), action == GLFW_PRESS || action == GLFW_REPEAT
            );
        }
    );
    glfwSetCharCallback(m_windowHandle, [](GLFWwindow *windowHandle, unsigned int c) {
        getEventQueue()->postCharEvent(c);
    });
    glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow *windowHandle, double x, double y) {
        GLFW_Window *self = static_cast<GLFW_Window *>(glfwGetWindowUserPointer(windowHandle));
        static double prevX = x;
        static double prevY = y;
        double dx = x - prevX;
        double dy = y - prevY;
        prevX = x;
        prevY = y;
        if (self->m_ignoreMouseEvents) {
            self->m_ignoreMouseEvents--;
            dx = 0;
            dy = 0;
        }
        getEventQueue()->postMouseEvent(x, y, dx, dy);
    });
    glfwSetScrollCallback(
        m_windowHandle,
        [](GLFWwindow *windowHandle, double xoffset, double yoffset) {
            getEventQueue()->postScrollEvent(xoffset, yoffset, false);
        }
    );
    glfwSetMouseButtonCallback(
        m_windowHandle,
        [](GLFWwindow *windowHandle, int button, int action, int mods) {
            getEventQueue()->postMouseButtonEvent(
                static_cast<MouseButton>(button), action == GLFW_PRESS
            );
        }
    );
    glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *windowHandle) {
        GLFW_Window *self = static_cast<GLFW_Window *>(glfwGetWindowUserPointer(windowHandle));
        getEventQueue()->postWindowCloseRequest(self);
    });
    int width, height;
    glfwGetWindowSize(m_windowHandle, &width, &height);
    getEventQueue()->postSizeEvent(width, height);
}

void GLFW_Window::ignoreNextMouseEvents() {
    m_ignoreMouseEvents = 3;
}

} // namespace Fern
