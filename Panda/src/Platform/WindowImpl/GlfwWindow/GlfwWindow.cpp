//
// Created by Admin on 26.01.2022.
//

#include "pndpch.hpp"
#include "GlfwWindow.hpp"
#include "Panda/Application/PlatformData.hpp"

namespace Panda {

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(m_windowHandle);
    glfwTerminate();
}

void glfwErrorCallback(int error_code, const char *description) {
    PND_ERROR("GLFW ERROR, code: {}, description: {}", error_code, description);
}

void GlfwWindow::initialize(const char *title, GSize size, bool isFullscreen) {
    m_isFullScreen = isFullscreen;
    m_windowSizeBackup = size;

    context = &ApplicationContext::get();

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

    // Create the window
    m_windowHandle = glfwCreateWindow((int)size.width, (int)size.height, title, nullptr, nullptr);
    if (m_windowHandle == nullptr) {
        PND_CRITICAL("Failed to create the GLFW window");
        exit(1);
    }
    PlatformData::get().nativeWindowHandle = m_windowHandle;

    glfwShowWindow(m_windowHandle);
    glfwFocusWindow(m_windowHandle);

    if (isFullscreen) {
        setFullScreen(true);
    }

    addEventHandlers();
}

bool GlfwWindow::isFullScreen() {
    return m_isFullScreen;
}

void GlfwWindow::setFullScreen(bool isFullScreen) {}

GSize GlfwWindow::getWindowSize() {
    int width;
    int height;
    glfwGetWindowSize(m_windowHandle, &width, &height);
    return {(float)width, (float)height};
}

void GlfwWindow::addEventHandlers() {
    glfwSetWindowUserPointer(m_windowHandle, this);
    glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow* windowHandle, int width, int height) {
        GlfwWindow* self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->postSizeEvent(width, height);
    });
    glfwSetKeyCallback(m_windowHandle, [](GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
        GlfwWindow* self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->postKeyEvent(static_cast<Key>(key), action == GLFW_PRESS);
    });
    glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow* windowHandle, double x, double y) {
        GlfwWindow* self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->postMouseEvent(x, y);
    });
    glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow* windowHandle) {
        GlfwWindow* self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->context->isApplicationShouldClose = true;
    });
}

void GlfwWindow::pollEvents() {
    glfwPollEvents();
}

} // namespace Panda