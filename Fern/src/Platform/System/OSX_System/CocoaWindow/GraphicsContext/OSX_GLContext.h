#pragma once

#import "Fern/GraphicsContext/GraphicsContext.hpp"
#import <Cocoa/Cocoa.h>

class OSX_GLContext final : public Fern::GraphicsContext {
public:
    OSX_GLContext(NSView *view);
    void setCurrent() override;
    void releaseDrawingContext() override;
    void swapBuffers() override;
    void update() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    NSOpenGLContext *m_context;
};
