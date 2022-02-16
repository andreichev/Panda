//
// Created by Admin on 22.01.2022.
//
#include "pndpch.hpp"
#include "PlatformDetection.hpp"
#include "PlatformInit.hpp"

#ifdef PND_PLATFORM_MACOS
#    include "PlatformSpecificRealizations/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#    include "PlatformSpecificRealizations/EventsImpl/EventsGlfw/EventsGlfw.hpp"
#endif

namespace Panda {

IWindow *createWindow() {
#ifdef PND_PLATFORM_MACOS
    return new GlfwWindow();
#else
#    error "Unknown platform"
#endif
}

IEvents *createEvents() {
#ifdef PND_PLATFORM_MACOS
    return new EventsGlfw();
#else
#    error "Unknown platform"
#endif
}

} // namespace Panda
