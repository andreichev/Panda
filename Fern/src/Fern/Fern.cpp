#include "Fern/Fern.hpp"
#include "Fern/Platform/PlatformSystem.hpp"

#include <Foundation/PlatformDetection.hpp>

#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
#    include "Platform/System/GLFW_System/GLFW_System.hpp"
#elif defined(PLATFORM_MACOS)
#    include "Platform/System/OSX_System/OSX_System.hpp"
#elif defined(PLATFORM_IOS)
#    include "Platform/System/UIKitWindow/PandaWindowIOSImpl.hpp"
#    include "PlatformSystem.hpp"
#endif

namespace Fern {

static PlatformSystem *s_system = nullptr;

void initialize() {
    PND_ASSERT(s_system == nullptr, "FERN ALREADY INITIALIZED");
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
    s_system = F_NEW(Foundation::getAllocator(), GLFW_System);
#elif defined(PLATFORM_MACOS)
    s_system = F_NEW(Foundation::getAllocator(), OSX_System);
#elif defined(PLATFORM_IOS)
    s_system = F_NEW(Foundation::getAllocator(), IosSystem);
#else
#    error "Unknown platform"
#endif
}

void terminate() {
    PND_ASSERT(s_system != nullptr, "FERN IS NOT INITIALIZED");
    F_DELETE(Foundation::getAllocator(), s_system);
    s_system = nullptr;
}

Window *
createWindow(const char *title, Rect rect, WindowState state, DrawingContextType contextType) {
    return s_system->createWindow(title, rect, state, contextType);
}

void disposeWindow(Window *window) {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    s_system->disposeWindow(window);
}

void pollEvents() {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    s_system->pollEvents();
}

Vec2 getCursorPosition() {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    return s_system->getCursorPosition();
}

const char *getClipboardText() {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    return s_system->getClipboardText();
}

void setClipboardText(const char *text) {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    s_system->setClipboardText(text);
}

void toggleCursorLock() {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    s_system->toggleCursorLock();
}

bool isCursorLocked() {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    return s_system->isCursorLocked();
}

void setCursor(Cursor cursor) {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    s_system->setCursor(cursor);
}

EventQueue *getEventQueue() {
    PND_ASSERT(s_system != nullptr, "FERN NOT INITIALIZED");
    return s_system->getEventQueue();
}

} // namespace Fern
