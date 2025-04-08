//
//  WonderView.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

#import "WonderView.h"
#import "Fern/Events/EventQueue.hpp"

#import <Miren/Miren.hpp>
#import <OpenGLES/ES3/gl.h>

using namespace Fern;

namespace Fern {
EventQueue* getEventQueue();
}

@implementation WonderView {
    CADisplayLink* displayLink;
}

// MARK: - Init

- (instancetype)init {
    self = [super initWithFrame:UIScreen.mainScreen.bounds];
    [self commonInit];
    return self;
}

- (instancetype)deinit {
    NSLog(@"UIView deinit");
}

- (void)commonInit {
    [self setupPlatform];
    [self initDisplayLink];
}

- (void) setupPlatform {
    // self.backgroundColor = UIColor.redColor;
    [self setMultipleTouchEnabled:true];
    CGFloat scale = UIScreen.mainScreen.nativeScale;
    [self setContentScaleFactor:scale];
    CGFloat width = self.frame.size.width * scale;
    CGFloat height = self.frame.size.height * scale;
    NSLog(@"UIView initialized, width: %f, height: %f\n", width, height);
}

// MARK: - OpenGL stuff

+ (Class) layerClass {
    return [CAEAGLLayer class];
}

// MARK: - Action handlers

- (void) initDisplayLink {
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayRefreshed:)];
    displayLink.preferredFramesPerSecond = 60;
    [displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) displayRefreshed:(CADisplayLink*)displayLink {
    Miren::renderFrame();
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        CGPoint touchLocation = [touch locationInView:self];
        // touchLocation.x *= self.contentScaleFactor;
        // touchLocation.y *= self.contentScaleFactor;
        // printf("TOUCH BEGAN: %d\n", touch.hash);
        getEventQueue()->postTouchBeganEvent(int(touch.hash), touchLocation.x, touchLocation.y);
    }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        // printf("TOUCH ENDED: %d\n", touch.hash);
        getEventQueue()->postTouchEndedEvent(int(touch.hash));
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        CGPoint touchLocation = [touch locationInView:self];
        // touchLocation.x *= self.contentScaleFactor;
        // touchLocation.y *= self.contentScaleFactor;
        // printf("TOUCH MOVED: %d\n", touch.hash);
        getEventQueue()->postTouchMovedEvent(int(touch.hash), touchLocation.x, touchLocation.y);
    }
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches) {
        // printf("TOUCH CANCELLED: %d\n", touch.hash);
        getEventQueue()->postTouchEndedEvent(int(touch.hash));
    }
}

@end
