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

- (void)postWindowSize {
    CGRect frame = [[window->getNative() contentView] frame];
    uint32_t width = frame.size.width;
    uint32_t height = frame.size.height;
    getEventQueue()->postSizeEvent(width, height);
}

- (BOOL)windowShouldClose:(id)sender {
    getEventQueue()->postWindowCloseRequest(window);
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification {
    [self postWindowSize];
}

- (void)windowDidChangeOcclusionState:(NSNotification *)notification {
    [self postWindowSize];
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
    [self postWindowSize];
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
    [self postWindowSize];
}

@end
