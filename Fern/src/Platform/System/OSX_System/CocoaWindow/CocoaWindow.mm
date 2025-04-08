#import "CocoaWindow.h"
#import "CocoaWindowDelegate.h"
#import "WonderView.h"

#import <Cocoa/Cocoa.h>

using namespace Fern;

namespace Fern {
void pollEvents();
}

@interface NativeWindow : NSWindow
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

CocoaWindow::CocoaWindow(
    const char *title,
    Fern::Rect _rect,
    Fern::WindowState state,
    Fern::DrawingContextType contextType
)
    : m_isCursorLocked(false)
    , m_handle(nil) {
    NSRect rect = CGRectMake(_rect.origin.x, _rect.origin.y, _rect.size.width, _rect.size.height);

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
    [window makeFirstResponder:gameView];
    [window setAcceptsMouseMovedEvents:YES];
    [window setRestorable:NO];
    if ([window respondsToSelector:@selector(setTabbingMode:)]) {
        [window setTabbingMode:NSWindowTabbingModeDisallowed];
    }
    [window makeKeyAndOrderFront:nil];
    m_graphicsContext = new OSX_GLContext([window contentView]);
    this->m_handle = window;
}

CocoaWindow::~CocoaWindow() {
    [m_handle orderOut:nil];
    [m_handle setDelegate:nil];
    [m_handle release];
    [[m_handle contentView] release];
    [m_handle close];
    m_handle = nil;
    delete m_graphicsContext;
    // HACK: Allow Cocoa to catch up before returning
    pollEvents();
}

void CocoaWindow::setTitle(const char *title) {
    [m_handle setTitle:@(title)];
}

WindowState CocoaWindow::getState() {
    NSUInteger masks = m_handle.styleMask;
    if (masks & NSWindowStyleMaskFullScreen) {
        return WindowState::WindowStateFullScreen;
    }
    if (m_handle.isMiniaturized) {
        return WindowState::WindowStateMinimized;
    }
    if (m_handle.isZoomed) {
        return WindowState::WindowStateMaximized;
    }
    return WindowState::WindowStateNormal;
}

void CocoaWindow::setState(Fern::WindowState state) {
    switch (state) {
        case WindowState::WindowStateMinimized:
            [m_handle miniaturize:nil];
            break;
        case WindowState::WindowStateMaximized:
            [m_handle zoom:nil];
            break;
        case WindowState::WindowStateFullScreen: {
            const NSUInteger masks = m_handle.styleMask;
            if (!(masks & NSWindowStyleMaskFullScreen)) {
                [m_handle toggleFullScreen:nil];
            }
            break;
        }
        case WindowState::WindowStateNormal:
        default: {
            const NSUInteger masks = m_handle.styleMask;
            if (masks & NSWindowStyleMaskFullScreen) {
                [m_handle toggleFullScreen:nil];
            } else if (m_handle.isMiniaturized) {
                [m_handle deminiaturize:nil];
            } else if (m_handle.isZoomed) {
                [m_handle zoom:nil];
            }
        }
        break;
    }
}

bool CocoaWindow::getModifiedState() {
    return m_handle.documentEdited;
}

void CocoaWindow::setModifiedState(bool isUnsavedChanges) {
    m_handle.documentEdited = isUnsavedChanges;
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

void CocoaWindow::setSize(Fern::Size size) {
    NSRect contentRect = [m_handle contentRectForFrameRect:[m_handle frame]];
    contentRect.origin.y += contentRect.size.height - size.height;
    contentRect.size = NSMakeSize(size.width, size.height);
    [m_handle setFrame:[m_handle frameRectForContentRect:contentRect]
                        display:YES];
}

Fern::Size CocoaWindow::getSize() {
    const NSRect contentRect = [[m_handle contentView] frame];
    float width = (float) contentRect.size.width;
    float height = (float) contentRect.size.height;
    return {width, height};
}

Fern::Size CocoaWindow::getDpi() {
    NSScreen* screen = [NSScreen mainScreen];
    float scale = (float) screen.backingScaleFactor;
    return {scale, scale};
}

Vec2 CocoaWindow::getCursorPosition() {
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

bool CocoaWindow::setProgressBar(float progress) {
    return true;
}

bool CocoaWindow::endProgressBar() {
    return true;
}

GraphicsContext* CocoaWindow::getDrawingContext() {
    return m_graphicsContext;
}

NativeWindow* CocoaWindow::getNative() {
    return m_handle;
}
