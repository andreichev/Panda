//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Fern/GraphicsContext/GraphicsContext.hpp"

@class EAGLContext;
@class CAEAGLLayer;

class GLES_Context final : public Fern::GraphicsContext {
public:
    GLES_Context(CAEAGLLayer *eaglLayer);
    void setCurrent() override;
    void releaseDrawingContext() override;
    void swapBuffers() override;
    void update() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    EAGLContext *m_context;
};
