//
//  WonderView.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

#include "Panda/Platform/PlatformData.hpp"

#import <OpenGLES/ES3/gl.h>
#import "WonderView.hpp"

@implementation WonderView {
    CADisplayLink* displayLink;
}

// MARK: - Init

- (instancetype)init
{
    self = [super initWithFrame:UIScreen.mainScreen.bounds];
    [self commonInit];
    return self;
}

- (void)commonInit {
    [self setupStyle];
    [self addActionHandlers];
}

- (void) setupStyle {    
    [self setContentScaleFactor:UIScreen.mainScreen.nativeScale];



    GLsizei width = self.frame.size.width * self.contentScaleFactor;
    GLsizei height = self.frame.size.height * self.contentScaleFactor;

    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)self.layer];
    
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking : @NO,
                                     kEAGLDrawablePropertyColorFormat     : kEAGLColorFormatRGBA8 };
    eaglLayer.opaque = NO;
    printf("OpenGLES initialized, width: %d, height: %d\n", width, height);
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
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
