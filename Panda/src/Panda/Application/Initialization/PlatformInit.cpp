//
// Created by Admin on 22.01.2022.
//
#include "pndpch.hpp"
#include "Panda/Application/PlatformDetection.hpp"
#include "PlatformInit.hpp"

#ifdef PND_PLATFORM_MACOS
#    include "Platform/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#elif defined(PND_PLATFORM_IOS)
#    include "Platform/WindowImpl/UIKitWindow/UIKitWindow.hpp"
#endif

namespace Panda {

Window *createWindow(ApplicationStartupSettings &settings) {
#ifdef PND_PLATFORM_DESKTOP
    return new GlfwWindow(settings.windowTitle, settings.windowSize, settings.isFullScreen);
#elif defined(PND_PLATFORM_IOS)
    return new UIKitWindow();
#else
#    error "Unknown platform"
#endif
}

} // namespace Panda
