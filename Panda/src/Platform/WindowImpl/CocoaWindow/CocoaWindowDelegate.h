#import <Cocoa/Cocoa.h>
#import <Panda/Events/EventQueue.hpp>

class CocoaWindow;

@interface CocoaWindowDelegate : NSObject<NSWindowDelegate> {
    Panda::EventQueue* eventQueue;
    CocoaWindow* window;
}

- (instancetype)initWithCocoa:(CocoaWindow *)initWindow;

- (void)setEventQueue:(Panda::EventQueue*)queue;

@end

