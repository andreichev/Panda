//
// Created by Admin on 22.01.2022.
//
#include "pndpch.hpp"
#include "PlatformDetection.hpp"
#include "PlatformInit.hpp"

#ifdef PND_PLATFORM_MACOS
#    include "PlatformSpecificRealizations/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#    include "PlatformSpecificRealizations/EventsImpl/EventsGlfw/EventsGlfw.hpp"
#elif defined(PND_PLATFORM_IOS)
#    include "PlatformSpecificRealizations/WindowImpl/UIKitWindow/UIKitWindow.hpp"
#    include "PlatformSpecificRealizations/EventsImpl/UIKitEvents/UIKitEvents.hpp"
#endif

namespace Panda {

IWindow *createWindow() {
#ifdef PND_PLATFORM_MACOS
    return new GlfwWindow();
#elif defined(PND_PLATFORM_IOS)
    return new UIKitWindow();
#else
#    error "Unknown platform"
#endif
}

IEvents *createEvents() {
#ifdef PND_PLATFORM_MACOS
    return new EventsGlfw();
#elif defined(PND_PLATFORM_IOS)
    return new UIKitEvents();
#else
#    error "Unknown platform"
#endif
}

} // namespace Panda
