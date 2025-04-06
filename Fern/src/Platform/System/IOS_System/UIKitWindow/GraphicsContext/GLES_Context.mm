//
// Created by Admin on 11.02.2022.
//

#include "GLES_Context.h"

#import <UIKit/UIKit.h>
#import <OpenGLES/ES3/gl.h>

GLuint colorRenderBuffer;
GLuint depthRenderBuffer;
GLuint frameBuffer;

GLES_Context::GLES_Context(CAEAGLLayer *eaglLayer) {
    m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    [EAGLContext setCurrentContext: m_context];

    glGenRenderbuffers(1, &colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

    [m_context renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];
    eaglLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking : @NO,
                                     kEAGLDrawablePropertyColorFormat     : kEAGLColorFormatRGBA8 };
    eaglLayer.opaque = YES;

    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

    int width = UIScreen.mainScreen.bounds.size.width * UIScreen.mainScreen.scale;
    int height = UIScreen.mainScreen.bounds.size.height * UIScreen.mainScreen.scale;
    NSLog(@"Screen size: %d x %d\n", width, height);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"Framebuffer is incomplete!");
    }
}

void GLES_Context::setCurrent() {
    [EAGLContext setCurrentContext: m_context];
}

void GLES_Context::releaseDrawingContext() {
    [EAGLContext setCurrentContext: nil];
}

void GLES_Context::swapBuffers() {
    [m_context presentRenderbuffer: GL_RENDERBUFFER];
}

void GLES_Context::update() {}

uint32_t GLES_Context::getDefaultFrameBufferId() {
    return frameBuffer;
}
