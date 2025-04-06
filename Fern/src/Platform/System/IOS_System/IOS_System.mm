#import "IOS_System.hpp"
#import "UIKitWindow/UIKitWindow.h"
#import "UIKitWindow/WonderView.h"
#import "UIKitWindow/WonderController.h"

#import <Foundation/Allocator.hpp>

extern std::filesystem::path g_ios_resources_path;

namespace Fern {

IOS_System::IOS_System() {}

Window *IOS_System::createWindow(const char *title, Rect rect, WindowState state, DrawingContextType contextType) {
    return F_NEW(Foundation::getAllocator(), UIKitWindow)(contextType);
}

void IOS_System::disposeWindow(Window *window) {
    F_DELETE(Foundation::getAllocator(), window);
}

void IOS_System::pollEvents() {}

Vec2 IOS_System::getCursorPosition() const {}

const char* IOS_System::getClipboardText() {}

void IOS_System::setClipboardText(const char *text) {}

void IOS_System::toggleCursorLock() {}

bool IOS_System::isCursorLocked() {
    return false;
}

void IOS_System::setCursor(Cursor cursor) {}

uint64_t IOS_System::getMilliSeconds() const {
    return (uint64_t)([[NSProcessInfo processInfo] systemUptime] * 1000);
}

const std::filesystem::path& IOS_System::getResourcesPath() {
    return g_ios_resources_path;
}

} // namespace Fern
