//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Fern/GraphicsContext/GraphicsContext.hpp"

namespace Fern {

class GlesContext : public GraphicsContext {
    GlesContext(CAEAGLLayer *eaglLayer);
    void swapBuffers() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    EAGLContext *context;
};

} // namespace Fern
