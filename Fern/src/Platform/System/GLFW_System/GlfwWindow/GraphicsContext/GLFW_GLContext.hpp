//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Fern/GraphicsContext/GraphicsContext.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Fern {

// GLFW and GLAD based context
class GLFW_GLContext : public GraphicsContext {
public:
    GLFW_GLContext(GLFWwindow *windowHandle);
    void setCurrent() override;
    void releaseDrawingContext() override;
    void swapBuffers() override;
    void update() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    GLFWwindow *m_windowHandle;
};

} // namespace Fern
