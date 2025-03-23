//
// Created by Admin on 11.02.2022.
//

#include "GLFW_GLContext.hpp"

#include <Foundation/Logger.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Fern {

void GLFW_GLContext::create() {
    glfwWindowHandle = PlatformData::get()->nativeWindowHandle;
    glfwMakeContextCurrent((GLFWwindow *)glfwWindowHandle);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
        LOG_CRITICAL("Failed to initialize GLAD");
    }
    // Enable v-sync
    glfwSwapInterval(1);
}

void GLFW_GLContext::flip() {
    glfwSwapBuffers((GLFWwindow *)glfwWindowHandle);
}

uint32_t GLFW_GLContext::getDefaultFrameBufferId() {
    return 0;
}

} // namespace Fern