//
// Created by Admin on 26.01.2022.
//

#include "pndpch.hpp"
#include "GlfwWindow.hpp"
#include "Panda/Application/PlatformData.hpp"

namespace Panda {

void glfwErrorCallback(int error_code, const char *description) {
    PND_ERROR("GLFW ERROR, code: {}, description: {}", error_code, description);
}

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(m_windowHandle);
    glfwTerminate();
}

GlfwWindow::GlfwWindow(const char *title, GSize size, bool isFullscreen) {
    m_isFullScreen = isFullscreen;
    m_windowSizeBackup = size;
    cursorLocked = false;

    context = ApplicationContext::get();

    PND_INFO("Hello GLFW! {}", glfwGetVersionString());
    glfwSetErrorCallback(glfwErrorCallback);

    // Initialize GLFW. Most GLFW functions will not work before doing this.
    if (glfwInit() == false) {
        PND_CRITICAL("GLFW INITIALIZATION ERROR");
        exit(1);
    }

    // Configure GLFW
    glfwDefaultWindowHints(); // optional, the current window hints are already the default

#ifdef PND_PLATFORM_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // the window will be resizable
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    // Create the window
    m_windowHandle = glfwCreateWindow((int)size.width, (int)size.height, title, nullptr, nullptr);
    if (m_windowHandle == nullptr) {
        PND_CRITICAL("Failed to create the GLFW window");
        exit(1);
    }
    PlatformData::get().nativeWindowHandle = m_windowHandle;

    glfwShowWindow(m_windowHandle);
    glfwFocusWindow(m_windowHandle);

    context->postSizeEvent(size.width, size.height);
    if (isFullscreen) {
        setFullScreen(true);
    }

    addEventHandlers();
}

bool GlfwWindow::isFullScreen() {
    return m_isFullScreen;
}

void GlfwWindow::setFullScreen(bool isFullScreen) {
    m_isFullScreen = isFullScreen;
    // Get primary monitor handle
    GLFWmonitor *monitorHandle = glfwGetPrimaryMonitor();
    // Get the resolution of the primary monitor
    const GLFWvidmode *vidmode = glfwGetVideoMode(monitorHandle);
    if (isFullScreen) {
        glfwSetWindowMonitor(m_windowHandle, monitorHandle, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
    } else {
        glfwSetWindowMonitor(m_windowHandle, nullptr, 0, 0, m_windowSizeBackup.width, m_windowSizeBackup.height, vidmode->refreshRate);
        // Center the window
        glfwSetWindowPos(
            m_windowHandle, (vidmode->width - m_windowSizeBackup.width) / 2, (vidmode->height - m_windowSizeBackup.height) / 2);
    }
}

void GlfwWindow::addEventHandlers() {
    glfwSetWindowUserPointer(m_windowHandle, this);
    glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow *windowHandle, int width, int height) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->m_windowSizeBackup = GSize(width, height);
        self->context->postSizeEvent(width, height);
    });
    glfwSetKeyCallback(m_windowHandle, [](GLFWwindow *windowHandle, int key, int scancode, int action, int mods) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->postKeyEvent(static_cast<Key>(key), action == GLFW_PRESS || action == GLFW_REPEAT);
    });
    glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow *windowHandle, double x, double y) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->postMouseEvent(x, y);
    });
    glfwSetMouseButtonCallback(m_windowHandle, [](GLFWwindow *windowHandle, int button, int action, int mods) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->postMouseButtonEvent(static_cast<MouseButton>(button), action == GLFW_PRESS);
    });
    glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *windowHandle) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->isApplicationShouldClose = true;
    });
}

void GlfwWindow::pollEvents() {
    glfwPollEvents();
}

bool GlfwWindow::isCursorLocked() {
    return cursorLocked;
}

void GlfwWindow::toggleCursorLock() {
    cursorLocked = cursorLocked == false;
    resetCursorPos();
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void GlfwWindow::resetCursorPos() {
    int x = m_windowSizeBackup.width / 2;
    int y = m_windowSizeBackup.height / 2;
    glfwSetCursorPos(m_windowHandle, x, y);
    context->postMouseEvent(x, y);
}

} // namespace Panda
