#import "CocoaWindow.hpp"
#import "CocoaWindowDelegate.h"
#import "WonderView.h"

#import <Foundation/Foundation.hpp>
#import <Miren/Miren.hpp>
#import <Miren/PlatformData.hpp>
#import <Cocoa/Cocoa.h>

using namespace Panda;

@interface NativeWindow : NSWindow {}
- (BOOL)isFullScreen;
@end

@implementation NativeWindow

- (BOOL)isWindowResizable {
    return (([self styleMask] & NSWindowStyleMaskResizable) == NSWindowStyleMaskResizable);
}

- (BOOL)isFullScreen {
    return (([self styleMask] & NSWindowStyleMaskFullScreen) == NSWindowStyleMaskFullScreen);
}

- (BOOL)canBecomeKeyWindow {
    // Required for NSWindowStyleMaskBorderless windows
    return YES;
}

- (BOOL)canBecomeMainWindow {
    return YES;
}

@end

CocoaWindow::CocoaWindow(const char *title, Panda::Size size, bool isFullscreen, bool isMaximized)
    : m_isCursorLocked(false)
    , m_handle(nil)
    , m_eventQueue(nullptr) {
    NSRect rect = CGRectMake(0, 0, size.width, size.height);

    NSUInteger styleMask = NSWindowStyleMaskMiniaturizable;
    styleMask |= NSWindowStyleMaskTitled;
    styleMask |= NSWindowStyleMaskClosable;
    styleMask |= NSWindowStyleMaskResizable;
    // styleMask |= NSWindowStyleMaskBorderless;

    NativeWindow* window = [[NativeWindow alloc] initWithContentRect:rect
                                                   styleMask:styleMask
                                                     backing:NSBackingStoreBuffered
                                                       defer:YES];
    [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
    [window setTitle:@(title)];
    CocoaWindowDelegate* delegate = [[CocoaWindowDelegate alloc] initWithCocoa:this];
    [window setDelegate:delegate];
    [window setHasShadow:NO];
    [window center];
    [window setBackgroundColor:[NSColor blackColor]];
    WonderView* gameView = [[WonderView alloc] init];
    [window setContentView:gameView];
    [gameView setWantsLayer:YES];
    [gameView setAcceptsTouchEvents:YES];
    [window makeFirstResponder:gameView];
    [window setAcceptsMouseMovedEvents:YES];
    [window setRestorable:NO];
    if ([window respondsToSelector:@selector(setTabbingMode:)]) {
        [window setTabbingMode:NSWindowTabbingModeDisallowed];
    }
    [window makeKeyAndOrderFront:nil];
    Miren::PlatformData::get()->nativeWindowHandle = window;
    this->m_handle = window;
}

CocoaWindow::~CocoaWindow() {
    [m_handle orderOut:nil];
    [m_handle setDelegate:nil];
    [m_handle release];
    [[m_handle contentView] release];
    [m_handle close];
    m_handle = nil;
    // HACK: Allow Cocoa to catch up before returning
    pollEvents();
}

bool CocoaWindow::isFullScreen() {
    return [m_handle isFullScreen];
}

void CocoaWindow::setFullScreen(bool isFullScreen) {
    bool currentlyFullScreen = [m_handle isFullScreen];
    if((isFullScreen && currentlyFullScreen) || (!isFullScreen && !currentlyFullScreen)) {
        return;
    }
    if(isFullScreen) { setMaximized(true); }
    [m_handle toggleFullScreen:nil];
}

void CocoaWindow::setTitle(const char *title) {
    [m_handle setTitle:@(title)];
}

bool CocoaWindow::isMaximized() {
    return [m_handle isZoomed];
}

void CocoaWindow::setMaximized(bool isMaximized) {
    if (![m_handle isZoomed]) {
        [m_handle zoom:nil];
    }
}

void CocoaWindow::setResizable(bool isResizable) {
    const NSUInteger styleMask = [m_handle styleMask];
    if (isResizable) {
        [m_handle setStyleMask:(styleMask | NSWindowStyleMaskResizable)];
        const NSWindowCollectionBehavior behavior =
            NSWindowCollectionBehaviorFullScreenPrimary |
            NSWindowCollectionBehaviorManaged;
        [m_handle setCollectionBehavior:behavior];
    } else {
        [m_handle setStyleMask:(styleMask & ~NSWindowStyleMaskResizable)];
        const NSWindowCollectionBehavior behavior =
        NSWindowCollectionBehaviorFullScreenPrimary;
        [m_handle setCollectionBehavior:behavior];
    }
}

void CocoaWindow::setSize(Panda::Size size) {
    NSRect contentRect = [m_handle contentRectForFrameRect:[m_handle frame]];
    contentRect.origin.y += contentRect.size.height - size.height;
    contentRect.size = NSMakeSize(size.width, size.height);
    [m_handle setFrame:[m_handle frameRectForContentRect:contentRect]
                        display:YES];
}

void CocoaWindow::pollEvents() {
    NSApplication* application = [NSApplication sharedApplication];
    while (true) {
        NSEvent *event = [application nextEventMatchingMask:NSEventMaskAny
                                                  untilDate:[NSDate distantPast]
                                                     inMode:NSDefaultRunLoopMode
                                                    dequeue:YES];
        if (!event) { break; }
        [application sendEvent:event];
        [application updateWindows];
    }
}

void CocoaWindow::toggleCursorLock() {
    m_isCursorLocked = !m_isCursorLocked;
}

bool CocoaWindow::isCursorLocked() {
    return m_isCursorLocked;
}

void CocoaWindow::setCursor(Cursor cursor) {}

void CocoaWindow::setEventQueue(EventQueue *eventQueue) {
    m_eventQueue = eventQueue;
    [[m_handle delegate] setEventQueue:eventQueue];
    [[m_handle contentView] setEventQueue:eventQueue];
}

Panda::Size CocoaWindow::getSize() {
    const NSRect contentRect = [[m_handle contentView] frame];
    float width = (float) contentRect.size.width;
    float height = (float) contentRect.size.height;
    return {width, height};
}

Panda::Size CocoaWindow::getDpi() {
    NSScreen* screen = [NSScreen mainScreen];
    float scale = (float) screen.backingScaleFactor;
    return {scale, scale};
}

const char * CocoaWindow::getClipboardText() {}

void CocoaWindow::setClipboardText(const char *text) {}

Vec2 CocoaWindow::getCursorPos() {
    NSRect  originalFrame = [m_handle frame];
    NSPoint location      = [m_handle mouseLocationOutsideOfEventStream];
    NSRect  adjustFrame   = [m_handle contentRectForFrameRect: originalFrame];

    int32_t x = location.x;
    int32_t y = int32_t(adjustFrame.size.height) - int32_t(location.y);

    // clamp within the range of the window
    x = Foundation::clamp(x, 0, int32_t(adjustFrame.size.width) );
    y = Foundation::clamp(y, 0, int32_t(adjustFrame.size.height) );
    return Vec2(x, y);
}
