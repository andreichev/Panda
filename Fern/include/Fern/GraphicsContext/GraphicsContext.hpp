//
// Created by Admin on 18.02.2022.
//

#pragma once

#include <cstdint>

namespace Fern {
class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    // Activates the drawing context.
    virtual void setCurrent() = 0;
    // Release the drawing context of the calling thread.
    virtual void releaseDrawingContext() = 0;
    virtual void swapBuffers() = 0;
    virtual void update() = 0;
    virtual uint32_t getDefaultFrameBufferId() = 0;
};
} // namespace Fern
