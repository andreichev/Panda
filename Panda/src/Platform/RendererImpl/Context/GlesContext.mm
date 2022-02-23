//
// Created by Admin on 11.02.2022.
//

#include "GlesContext.hpp"

#include "Panda/Application/PlatformData.hpp"

namespace Panda {

    void GlesContext::create(int width, int height) {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        CAEAGLLayer* layer = (__bridge CAEAGLLayer*) PlatformData::get().nativeWindowHandle;

        layer.opaque = [layer.style valueForKey:@"opaque"] == nil ? true : [[layer.style valueForKey:@"opaque"] boolValue];

        layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys
        : [NSNumber numberWithBool:false]
                , kEAGLDrawablePropertyRetainedBacking
                , kEAGLColorFormatRGBA8
                , kEAGLDrawablePropertyColorFormat
                , nil
        ];

        [EAGLContext setCurrentContext:context];

        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glGenRenderbuffers(1, &colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);

        GLint width;
        GLint height;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
        PND_INFO("Screen size: {} x {}", width, height);

        glGenRenderbuffers(1, &m_depthStencilRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRbo);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

}