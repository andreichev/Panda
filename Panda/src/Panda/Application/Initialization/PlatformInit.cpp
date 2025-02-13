//
// Created by Admin on 22.01.2022.
//

#include "pndpch.hpp"
#include "PlatformInit.hpp"

#include <Foundation/Allocator.hpp>
#include <Foundation/PlatformDetection.hpp>

#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
#    include "Platform/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#elif defined(PLATFORM_MACOS)
#    include "Platform/WindowImpl/CocoaWindow/CocoaWindow.hpp"
#elif defined(PLATFORM_IOS)
#    include "Platform/WindowImpl/UIKitWindow/PandaWindowIOSImpl.hpp"
#endif

namespace Panda {

Window *createWindow(ApplicationStartupSettings &settings) {
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
    return F_NEW(Foundation::getAllocator(), GlfwWindow)(
        settings.windowTitle, settings.windowSize, settings.isFullScreen, settings.isMaximized
    );
#elif defined(PLATFORM_MACOS)
    return F_NEW(Foundation::getAllocator(), CocoaWindow)(
        settings.windowTitle, settings.windowSize, settings.isFullScreen, settings.isMaximized
    );
#elif defined(PLATFORM_IOS)
    return F_NEW(Foundation::getAllocator(), PandaWindowIOSImpl);
#else
#    error "Unknown platform"
#endif
}

} // namespace Panda
