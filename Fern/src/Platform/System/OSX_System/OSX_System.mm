#import "OSX_System.hpp"
#import "CocoaWindow/CocoaWindow.h"
#import "CocoaWindow/WonderView.h"

#import <Foundation/Allocator.hpp>
#import <Foundation/IO/FileStreamReader.hpp>
#import <Cocoa/Cocoa.h>

namespace Fern {

static NSCursor *s_cursors[Cursor::COUNT];

OSX_System::OSX_System() : m_isCursorLocked(false) {
    s_cursors[Cursor::ARROW] = [NSCursor arrowCursor];
    s_cursors[Cursor::IBEAM] = [NSCursor IBeamCursor];
    s_cursors[Cursor::CROSSHAIR] = [NSCursor crosshairCursor];
    s_cursors[Cursor::POINTING_HAND] = [NSCursor pointingHandCursor];
    s_cursors[Cursor::RESIZE_EW] = [NSCursor resizeLeftRightCursor];
    s_cursors[Cursor::RESIZE_NS] = [NSCursor resizeUpDownCursor];
    s_cursors[Cursor::RESIZE_NESW] = nil;
    s_cursors[Cursor::RESIZE_NWSE] = nil;
    s_cursors[Cursor::RESIZE_ALL] = [NSCursor closedHandCursor];
    s_cursors[Cursor::NOT_ALLOWED] = [NSCursor operationNotAllowedCursor];
    
    SEL cursorNWSESelector = NSSelectorFromString(@"_windowResizeNorthWestSouthEastCursor");
    SEL cursorNESWSelector = NSSelectorFromString(@"_windowResizeNorthEastSouthWestCursor");
    if (cursorNWSESelector && [NSCursor respondsToSelector:cursorNWSESelector]) {
        s_cursors[Cursor::RESIZE_NWSE] = [NSCursor performSelector:cursorNWSESelector];
    } else {
        s_cursors[Cursor::RESIZE_NWSE] = [NSCursor crosshairCursor];
    }
    if (cursorNESWSelector && [NSCursor respondsToSelector:cursorNESWSelector]) {
        s_cursors[Cursor::RESIZE_NESW] = [NSCursor performSelector:cursorNESWSelector];
    } else {
        s_cursors[Cursor::RESIZE_NESW] = [NSCursor crosshairCursor];
    }
}

Window *OSX_System::createWindow(
    const char *title, Rect rect, WindowState state, DrawingContextType contextType
) {
    return F_NEW(Foundation::getAllocator(), CocoaWindow)(
        title, rect, state, contextType
    );
}

void OSX_System::disposeWindow(Window *window) {
    if (window->getState() == WindowState::WindowStateFullScreen) {
        window->setState(WindowState::WindowStateNormal);
    }
    F_DELETE(Foundation::getAllocator(), window);
}

void OSX_System::pollEvents() {
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

Vec2 OSX_System::getCursorPosition() const {
    const NSPoint mouseLoc = [NSEvent mouseLocation];
    return {static_cast<float>(mouseLoc.x), static_cast<float>(mouseLoc.y)};
}

const char * OSX_System::getClipboardText() {
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    if (![[pasteboard types] containsObject:NSPasteboardTypeString]) {
        return nullptr;
    }
    NSString* object = [pasteboard stringForType:NSPasteboardTypeString];
    if (!object) {
        return nullptr;
    }
    return [object UTF8String];
}

void OSX_System::setClipboardText(const char *text) {
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard declareTypes:@[NSPasteboardTypeString] owner:nil];
    [pasteboard setString:@(text) forType:NSPasteboardTypeString];
}

void OSX_System::toggleCursorLock() {
    m_isCursorLocked = !m_isCursorLocked;
    if (m_isCursorLocked) {
        CGAssociateMouseAndMouseCursorPosition(false);
        NSWindow* window = [NSApp keyWindow];
        if (!window) { return; }
        CGRect frame = window.frame;
        CGFloat height = window.screen.frame.size.height;
        CGPoint center = CGPointMake(frame.origin.x + frame.size.width / 2,
                                     height - (frame.origin.y + frame.size.height / 2));
        CGWarpMouseCursorPosition(center);
        // Игнорируем события мыши из за программного перемещения во избежания лишних дельт
        if ([window.contentView isKindOfClass:[WonderView class]]) {
            WonderView *view = (WonderView *)window.contentView;
            view.ignoreMouseEvents = 3;
        }
        [NSCursor hide];
    } else {
        CGAssociateMouseAndMouseCursorPosition(true);
        [NSCursor unhide];
    }
}

bool OSX_System::isCursorLocked() {
    return m_isCursorLocked;
}

void OSX_System::setCursor(Cursor cursor) {
    NSCursor *nsCursor = s_cursors[cursor];
    if (!nsCursor) { return; }
    [nsCursor set];
}

uint64_t OSX_System::getMilliSeconds() const {
    return (uint64_t)([[NSProcessInfo processInfo] systemUptime] * 1000);
}

const std::filesystem::path& OSX_System::getStaticResourcesPath() {
    static std::filesystem::path path;
    return path;
}

Foundation::StreamReaderI* OSX_System::createStaticResourceReader(const std::filesystem::path& path) {
    return F_NEW(Foundation::getAllocator(), Foundation::FileStreamReader)(path);
}

void OSX_System::disposeResourceReader(Foundation::StreamReaderI* reader) {
    F_DELETE(Foundation::getAllocator(), reader);
}
    
} // namespace Fern
