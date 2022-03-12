//
// Created by Admin on 11.02.2022.
//

#include "OpenGLContext.hpp"
#include "Panda/Application/PlatformData.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Panda {

void OpenGLContext::create(int width, int height) {
    glfwWindowHandle = PlatformData::get().nativeWindowHandle;
    glfwMakeContextCurrent((GLFWwindow *)glfwWindowHandle);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
        PND_CRITICAL("Failed to initialize GLAD");
    }
    // Enable v-sync
    glfwSwapInterval(1);
}

void OpenGLContext::beginFrame() {}

void OpenGLContext::endFrame() {
    glfwSwapBuffers((GLFWwindow *)glfwWindowHandle);
}

} // namespace Panda