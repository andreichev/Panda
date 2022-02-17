//
//  WonderView.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

#import "WonderView.hpp"

@implementation WonderView {
    // EAGLContext* context;
    CADisplayLink* displayLink;
    // RendererWrapper* renderer;
    // GLuint colorRenderBuffer;
    // GLuint depthRenderBuffer;
    // GLuint frameBuffer;
}
// MARK: - Init

- (instancetype)init
{
    self = [super initWithFrame:UIScreen.mainScreen.bounds];
    // context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    // colorRenderBuffer = 0;
    // depthRenderBuffer = 0;
    // frameBuffer = 0;
    [self commonInit];
    return self;
}

- (void)commonInit {
    [self setupStyle];
    [self addActionHandlers];
}

- (void) setupStyle {
    self.backgroundColor = UIColor.redColor;
//    [EAGLContext setCurrentContext:context];
//
//    [self setContentScaleFactor:UIScreen.mainScreen.nativeScale];
//
//    GLsizei width = self.frame.size.width * self.contentScaleFactor;
//    GLsizei height = self.frame.size.height * self.contentScaleFactor;
//
//    glGenRenderbuffers(1, &colorRenderBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
//    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)self.layer];
//
//    glGenRenderbuffers(1, &depthRenderBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
//
//    glGenFramebuffers(1, &frameBuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
//
//    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
//    eaglLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking : @NO,
//                                     kEAGLDrawablePropertyColorFormat     : kEAGLColorFormatSRGBA8 };
//    eaglLayer.opaque = YES;
//
//    renderer = [[RendererWrapper alloc] init];
//    [renderer updateScreenSize:width height:height];
}

// MARK: - OpenGL stuff

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

// MARK: - Action handlers

- (void) addActionHandlers {
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayRefreshed:)];
    displayLink.preferredFramesPerSecond = 60;
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) displayRefreshed:(CADisplayLink*)displayLink {
    // [renderer render:displayLink.timestamp];
    // glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    // [context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
