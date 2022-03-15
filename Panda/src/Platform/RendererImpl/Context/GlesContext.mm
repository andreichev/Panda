//
// Created by Admin on 11.02.2022.
//

#include "GlesContext.hpp"
#include "Panda/Application/PlatformData.hpp"

#import <UIKit/UIKit.h>
#import <OpenGLES/ES3/gl.h>

namespace Panda {
    GLuint colorRenderBuffer;
    GLuint depthRenderBuffer;
    GLuint frameBuffer;

    void GlesContext::create(int width, int height) {
//        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
//        CAEAGLLayer* layer = (__bridge CAEAGLLayer*) PlatformData::get().nativeWindowHandle;
//
//        layer.opaque = [layer.style valueForKey:@"opaque"] == nil ? true : [[layer.style valueForKey:@"opaque"] boolValue];
//
//        layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys
//        : [NSNumber numberWithBool:false]
//                , kEAGLDrawablePropertyRetainedBacking
//                , kEAGLColorFormatRGBA8
//                , kEAGLDrawablePropertyColorFormat
//                , nil
//        ];
//
//        EAGLContext* _context = (__bridge EAGLContext*) context;
//        [EAGLContext setCurrentContext: _context];
//
//        glGenFramebuffers(1, &frameBuffer);
//        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//
//        glGenRenderbuffers(1, &colorRenderBuffer);
//        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
//
//        dispatch_sync(dispatch_get_main_queue(), ^{
//            [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
//        });
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
//
//        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
//        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
//        PND_INFO("Screen size: {} x {}", width, height);
//
//        glGenRenderbuffers(1, &depthRenderBuffer);
//        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
//
//        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//        glViewport(0, 0, width, height);
//        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
//        glClear(GL_COLOR_BUFFER_BIT);

        // -------------------

        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        PlatformData::get().renderingContext = context;
        EAGLContext* _context = (__bridge EAGLContext*) context;
        [EAGLContext setCurrentContext:_context];

        glGenRenderbuffers(1, &colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        
        CAEAGLLayer *eaglLayer = (__bridge CAEAGLLayer*) PlatformData::get().layer;;
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>) eaglLayer];
        eaglLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking : @NO,
                                         kEAGLDrawablePropertyColorFormat     : kEAGLColorFormatRGBA8 };
        eaglLayer.opaque = NO;
        glViewport(0, 0, width, height);
        // glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GlesContext::flip() {
        EAGLContext* _context = (__bridge EAGLContext*) context;
        [_context presentRenderbuffer: GL_RENDERBUFFER];
    }
}
