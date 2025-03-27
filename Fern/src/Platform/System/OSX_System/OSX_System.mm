#import "OSX_System.hpp"
#import "CocoaWindow/CocoaWindow.h"

#import <Foundation/Allocator.hpp>
#import <Cocoa/Cocoa.h>

namespace Fern {

OSX_System::OSX_System() : m_isCursorLocked(false) {}

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
    delete window;
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
    CGAssociateMouseAndMouseCursorPosition(!m_isCursorLocked);
}

bool OSX_System::isCursorLocked() {
    return m_isCursorLocked;
}

void OSX_System::setCursor(Cursor cursor) {}

uint64_t OSX_System::getMilliSeconds() const {
    return (uint64_t)([[NSProcessInfo processInfo] systemUptime] * 1000);
}
    
} // namespace Fern
