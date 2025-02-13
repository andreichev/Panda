//
//  WonderView.mm
//
//  Created by Andreichev Mikhail on 07.02.2025.
//

#import "WonderView.h"

using namespace Panda;

@implementation WonderView

- (instancetype)init {
    self = [super init];
    NSLog(@"WonderView created");
    return self;
}

- (void)dealloc {
    [super dealloc];
    NSLog(@"WonderView deallocated");
}

- (NSTouchTypeMask)allowedTouchTypes {
    return NSTouchTypeIndirect;
}

- (void)setEventQueue:(Panda::EventQueue *)queue {
    eventQueue = queue;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyView {
    return YES;
}

- (BOOL)wantsUpdateLayer {
    return YES;
}

- (void)mouseMoved:(NSEvent *)event {
    // NOTE: The returned location uses base 0,1 not 0,0
    const NSPoint pos = [event locationInWindow];
    double xpos = pos.x;
    double ypos = self.frame.size.height - pos.y;
    eventQueue->postMouseEvent(xpos, ypos);
}

- (void)mouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}

- (void)rightMouseDragged:(NSEvent *)event {
    [self mouseMoved:event];
}

- (void)mouseDown:(NSEvent *)event {
    // Command + Left Mouse Button acts as middle! This just a temporary solution!
    // This is because the average OSX user doesn't have middle mouse click.
    MouseButton mb = ([event modifierFlags] & NSEventModifierFlagCommand)
    ? MouseButton::MIDDLE
    : MouseButton::LEFT
    ;
    eventQueue->postMouseButtonEvent(mb, true);
}

- (void)mouseUp:(NSEvent *)event {
    MouseButton mb = ([event modifierFlags] & NSEventModifierFlagCommand)
    ? MouseButton::MIDDLE
    : MouseButton::LEFT
    ;
    eventQueue->postMouseButtonEvent(mb, false);
}

- (void)rightMouseDown:(NSEvent *)event {
    eventQueue->postMouseButtonEvent(MouseButton::RIGHT, true);
}

- (void)rightMouseUp:(NSEvent *)event {
    eventQueue->postMouseButtonEvent(MouseButton::RIGHT, false);
}

- (void)otherMouseDown:(NSEvent *)event {
    eventQueue->postMouseButtonEvent(MouseButton::MIDDLE, true);
}

- (void)otherMouseUp:(NSEvent *)event {
    eventQueue->postMouseButtonEvent(MouseButton::RIGHT, false);
}

- (void)scrollWheel:(NSEvent *)event {
    double deltaX = [event scrollingDeltaX];
    double deltaY = [event scrollingDeltaY];
    if ([event hasPreciseScrollingDeltas]) {
        deltaX *= 0.1;
        deltaY *= 0.1;
    }
    eventQueue->postScrollEvent(deltaX, deltaY);
}

@end

