//
//  WonderView.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

#include "Panda/Application/PlatformData.hpp"
#include "Panda/Application/ApplicationContext.hpp"

#import <OpenGLES/ES3/gl.h>
#import "WonderView.hpp"

@implementation WonderView {
    CADisplayLink* displayLink;
    dispatch_semaphore_t semaphore;
}

// MARK: - Init

- (instancetype)init {
    self = [super initWithFrame:UIScreen.mainScreen.bounds];
    [self commonInit];
    return self;
}

- (void)commonInit {
    [self setupStyle];
    [self addActionHandlers];
}

- (void) setupStyle {
    // self.backgroundColor = UIColor.redColor;
    [self setContentScaleFactor:UIScreen.mainScreen.nativeScale];
    CGFloat scale = UIScreen.mainScreen.nativeScale;
    Panda::ApplicationContext::get().postSizeEvent(self.frame.size.width * scale, self.frame.size.height * scale);
    semaphore = dispatch_semaphore_create(1);
    Panda::PlatformData::get().layer = self.layer;
    Panda::PlatformData::get().nativeWindowHandle = self;
    Panda::PlatformData::get().semaphoreHandle = &semaphore;
}

// MARK: - OpenGL stuff

+ (Class) layerClass {
    return [CAEAGLLayer class];
}

// MARK: - Action handlers

- (void) addActionHandlers {
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayRefreshed:)];
    displayLink.preferredFramesPerSecond = 60;
    [displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) displayRefreshed:(CADisplayLink*)displayLink {
    void* context = Panda::PlatformData::get().renderingContext;
    if(context != nullptr) {
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        EAGLContext* _context = (__bridge EAGLContext*) context;
        [_context presentRenderbuffer: GL_RENDERBUFFER];
        dispatch_semaphore_signal(semaphore);
    }
}

@end
