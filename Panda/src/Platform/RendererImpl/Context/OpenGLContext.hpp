//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/GraphicsContext.hpp"

namespace Panda {

// GLFW and GLAD based context
class OpenGLContext : public GraphicsContext {
public:
    void create() override;
    void flip() override;

private:
    void *glfwWindowHandle;
};

} // namespace Panda
