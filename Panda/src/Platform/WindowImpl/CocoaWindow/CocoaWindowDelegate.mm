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

- (void)windowDidBecomeKey:(NSNotification *)notification {
    NSLog(@"WINDOW IS READY!");
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

- (void)windowDidMove:(NSNotification *)notification {

}

- (void)windowDidMiniaturize:(NSNotification *)notification {

}

- (void)windowDidDeminiaturize:(NSNotification *)notification {

}

- (void)windowDidResignKey:(NSNotification *)notification {

}

- (void)windowDidChangeOcclusionState:(NSNotification* )notification {
    
}

@end
