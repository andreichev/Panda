//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Fern/GraphicsContext/GraphicsContext.hpp"

namespace Fern {

// GLFW and GLAD based context
class GLFW_GLContext : public GraphicsContext {
public:
    void create() override;
    void flip() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    void *glfwWindowHandle;
};

} // namespace Fern
