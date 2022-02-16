//
// Created by Admin on 26.01.2022.
//

#include "pndpch.hpp"
#include "GlfwWindow.hpp"

namespace Panda {

void glfwErrorCallback(int error_code, const char *description) {
    PND_ERROR("GLFW ERROR, code: {}, description: {}", error_code, description);
}

void GlfwWindow::initialize(const char *title, GSize size, bool isFullscreen) {
    m_isFullScreen = isFullscreen;
    m_windowSizeBackup = size;

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

    // Make the OpenGL context current
    glfwMakeContextCurrent(m_windowHandle);
    // Enable v-sync
    glfwSwapInterval(1);

    glfwShowWindow(m_windowHandle);
    glfwFocusWindow(m_windowHandle);

    if (isFullscreen) {
        setFullScreen(true);
    }
}

double GlfwWindow::getTime() {
    return glfwGetTime();
}

bool GlfwWindow::isShouldClose() {
    return glfwWindowShouldClose(m_windowHandle);
}

void GlfwWindow::setShouldClose(bool flag) {
    glfwSetWindowShouldClose(m_windowHandle, flag);
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

void GlfwWindow::swapBuffers() {
    glfwSwapBuffers(m_windowHandle);
}

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(m_windowHandle);
    glfwTerminate();
}

void *GlfwWindow::getNativeWindow() {
    return m_windowHandle;
}
void *GlfwWindow::getProcAddressFuncPointer() {
    return (void *)glfwGetProcAddress;
}

} // namespace Panda