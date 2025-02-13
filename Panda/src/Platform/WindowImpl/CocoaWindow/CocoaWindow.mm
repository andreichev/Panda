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

    NativeWindow* window = [[NativeWindow alloc] initWithContentRect:rect
                                                   styleMask:styleMask
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    [window setTitle:@(title)];
    CocoaWindowDelegate* delegate = [[CocoaWindowDelegate alloc] initWithCocoa:this];
    [window setDelegate:delegate];
    [window makeKeyAndOrderFront:nil];
    [window setHasShadow:NO];
    [window center];
    [window setBackgroundColor:[NSColor blackColor]];

    WonderView* gameView = [[WonderView alloc] init];
    [window setContentView:gameView];
    [gameView setWantsLayer:YES];
    [window makeFirstResponder:gameView];
    [window setAcceptsMouseMovedEvents:YES];
    [window setRestorable:NO];
    if ([window respondsToSelector:@selector(setTabbingMode:)]) {
        [window setTabbingMode:NSWindowTabbingModeDisallowed];
    }
    Miren::PlatformData::get()->nativeWindowHandle = window;
    this->m_handle = window;
    // pollEvents();
}

CocoaWindow::~CocoaWindow() {}

bool CocoaWindow::isFullScreen() {
    return [m_handle isFullScreen];
}

void CocoaWindow::setFullScreen(bool isFullScreen) {
    bool currentlyFullScreen = [m_handle isFullScreen];
    if((isFullScreen && currentlyFullScreen) || (!isFullScreen && !currentlyFullScreen)) {
        return;
    }
    NSLog(@"TOGGLE FULL SCREEN!");
    [m_handle toggleFullScreen:nil];
}

void CocoaWindow::setTitle(const char *title) {
}

bool CocoaWindow::isMaximized() {
    return isFullScreen();
}

void CocoaWindow::setMaximized(bool isMaximized) {
    setFullScreen(isMaximized);
}

void CocoaWindow::setResizable(bool isResizable) {
}

void CocoaWindow::setSize(Panda::Size size) {
}

void CocoaWindow::pollEvents() {
    NSApplication* application = [NSApplication sharedApplication];
    while (true) {
        NSEvent *event = [application nextEventMatchingMask:NSEventMaskAny
                                                  untilDate:nil
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
