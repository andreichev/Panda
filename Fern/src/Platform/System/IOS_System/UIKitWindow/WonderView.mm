//
//  WonderView.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

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

- (instancetype)deinit {
    Miren::terminate();
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
    printf("UIView initialized, width: %f, height: %f\n", width, height);
    Miren::PlatformData::get()->layer = self.layer;
    Miren::PlatformData::get()->nativeWindowHandle = self;
    Miren::initialize();
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
    if (_eventQueue == NULL) { return; }
    for (UITouch *touch in touches) {
        CGPoint touchLocation = [touch locationInView:self];
        // touchLocation.x *= self.contentScaleFactor;
        // touchLocation.y *= self.contentScaleFactor;
        // printf("TOUCH BEGAN: %d\n", touch.hash);
        _eventQueue->postTouchBeganEvent(int(touch.hash), touchLocation.x, touchLocation.y);
    }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if (_eventQueue == NULL) { return; }
    for (UITouch *touch in touches) {
        // printf("TOUCH ENDED: %d\n", touch.hash);
        _eventQueue->postTouchEndedEvent(int(touch.hash));
    }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if (_eventQueue == NULL) { return; }
    for (UITouch *touch in touches) {
        CGPoint touchLocation = [touch locationInView:self];
        // touchLocation.x *= self.contentScaleFactor;
        // touchLocation.y *= self.contentScaleFactor;
        // printf("TOUCH MOVED: %d\n", touch.hash);
        _eventQueue->postTouchMovedEvent(int(touch.hash), touchLocation.x, touchLocation.y);
    }
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if (_eventQueue == NULL) { return; }
    for (UITouch *touch in touches) {
        // printf("TOUCH CANCELLED: %d\n", touch.hash);
        _eventQueue->postTouchEndedEvent(int(touch.hash));
    }
}

@end
