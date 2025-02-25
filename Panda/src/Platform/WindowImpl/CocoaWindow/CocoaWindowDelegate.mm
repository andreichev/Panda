#import "CocoaWindowDelegate.h"
#import "CocoaWindow.hpp"

@implementation CocoaWindowDelegate

- (instancetype)initWithCocoa:(CocoaWindow *)initWindow {
    self = [super init];
    if (self != nil)
        window = initWindow;
    return self;
}

- (void)setEventQueue:(Panda::EventQueue *)queue {
    eventQueue = queue;
}

- (BOOL)windowShouldClose:(id)sender {
    eventQueue->postWindowCloseEvent();
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification {
    CGRect frame = [[window->getNative() contentView] frame];
    uint32_t width = frame.size.width;
    uint32_t height = frame.size.height;
    eventQueue->postSizeEvent(width, height);
}

- (void)windowDidChangeOcclusionState:(NSNotification *)notification {
    Panda::Vec2 pos = window->getCursorPos();
    eventQueue->postMouseEvent(pos.x, pos.y);
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
    Panda::Vec2 pos = window->getCursorPos();
    eventQueue->postMouseEvent(pos.x, pos.y);
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
    Panda::Vec2 pos = window->getCursorPos();
    eventQueue->postMouseEvent(pos.x, pos.y);
}

@end
