//
//  WonderView.swift
//  Wonderland
//
//  Created by Admin on 02.08.2021.
//

#include "Panda/Application/PlatformData.hpp"
#include "Panda/Renderer/Miren.hpp"

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
    Panda::Miren::terminate();
}

- (void)commonInit {
    [self setupPlatform];
    [self initDisplayLink];
}

- (void) setupPlatform {
    // self.backgroundColor = UIColor.redColor;
    CGFloat scale = UIScreen.mainScreen.nativeScale;
    [self setContentScaleFactor:scale];
    CGFloat width = self.frame.size.width * scale;
    CGFloat height = self.frame.size.height * scale;
    printf("UIView initialized, width: %f, height: %f\n", width, height);
    Panda::PlatformData::get()->layer = self.layer;
    Panda::PlatformData::get()->nativeWindowHandle = self;
    Panda::Miren::initialize();
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
    Panda::Miren::renderFrame();
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_eventQueue == NULL) { return; }
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;
    _eventQueue->postMouseEvent(touchLocation.x, touchLocation.y);
    _eventQueue->postMouseButtonEvent(Panda::MouseButton::LEFT, true);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_eventQueue == NULL) { return; }
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;
    // TODO: Post mouse released event
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_eventQueue == NULL) { return; }
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;

    _eventQueue->postMouseEvent(touchLocation.x, touchLocation.y);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_eventQueue == NULL) { return; }
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    touchLocation.x *= self.contentScaleFactor;
    touchLocation.y *= self.contentScaleFactor;
    // TODO: Post mouse released event
}

@end
