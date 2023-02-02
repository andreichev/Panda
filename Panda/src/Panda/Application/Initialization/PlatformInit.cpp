//
// Created by Admin on 22.01.2022.
//

#include "pndpch.hpp"
#include "PlatformInit.hpp"

#include <Foundation/Allocator.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_DESKTOP
#    include "Platform/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#elif defined(PLATFORM_IOS)
#    include "Platform/WindowImpl/UIKitWindow/PandaWindowIOSImpl.hpp"
#endif

namespace Panda {

Window *createWindow(ApplicationStartupSettings &settings) {
#ifdef PLATFORM_DESKTOP
    return NEW(Foundation::getAllocator(), GlfwWindow)(
        settings.windowTitle, settings.windowSize, settings.isFullScreen);
#elif defined(PLATFORM_IOS)
    return NEW(Foundation::getAllocator(), PandaWindowIOSImpl);
#else
#    error "Unknown platform"
#endif
}

} // namespace Panda
