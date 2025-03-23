#import "CocoaWindowDelegate.h"
#import "CocoaWindow.h"

using namespace Fern;

namespace Fern {
EventQueue* getEventQueue();
}

@implementation CocoaWindowDelegate

- (instancetype)initWithCocoa:(CocoaWindow *)initWindow {
    self = [super init];
    if (self != nil) {
        window = initWindow;
    }
    return self;
}

- (BOOL)windowShouldClose:(id)sender {
    getEventQueue()->postWindowCloseEvent();
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification {
    CGRect frame = [[window->getNative() contentView] frame];
    uint32_t width = frame.size.width;
    uint32_t height = frame.size.height;
    getEventQueue()->postSizeEvent(width, height);
}

- (void)windowDidChangeOcclusionState:(NSNotification *)notification {
    Fern::Vec2 pos = window->getCursorPosition();
    getEventQueue()->postMouseEvent(pos.x, pos.y);
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
    Fern::Vec2 pos = window->getCursorPosition();
    getEventQueue()->postMouseEvent(pos.x, pos.y);
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
    Fern::Vec2 pos = window->getCursorPosition();
    getEventQueue()->postMouseEvent(pos.x, pos.y);
}

@end
