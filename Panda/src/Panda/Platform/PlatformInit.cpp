//
// Created by Admin on 22.01.2022.
//
#include "pndpch.hpp"
#include "PlatformDetection.hpp"
#include "PlatformInit.hpp"

#ifdef PND_PLATFORM_MACOS
#    include "Panda/Window/Implementation/GlfwWindow/GlfwWindow.hpp"
#    include "Panda/Events/Implementation/EventsGlfw/EventsGlfw.hpp"
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