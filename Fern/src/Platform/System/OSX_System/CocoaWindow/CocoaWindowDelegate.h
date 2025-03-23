#import <Cocoa/Cocoa.h>
#import <Fern/Events/EventQueue.hpp>

class CocoaWindow;

@interface CocoaWindowDelegate : NSObject<NSWindowDelegate> {
    CocoaWindow* window;
}

- (instancetype)initWithCocoa:(CocoaWindow *)initWindow;

@end

