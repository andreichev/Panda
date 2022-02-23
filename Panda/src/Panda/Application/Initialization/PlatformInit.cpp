//
// Created by Admin on 22.01.2022.
//
#include "pndpch.hpp"
#include "Panda/Application/PlatformDetection.hpp"
#include "PlatformInit.hpp"

#ifdef PND_PLATFORM_MACOS
#    include "Platform/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#elif defined(PND_PLATFORM_IOS)
#    include "PlatformSpecificRealizations/WindowImpl/UIKitWindow/UIKitWindow.hpp"
#endif

namespace Panda {

Window *createWindow() {
#ifdef PND_PLATFORM_MACOS
    return new GlfwWindow();
#elif defined(PND_PLATFORM_IOS)
    return new UIKitWindow();
#else
#    error "Unknown platform"
#endif
}

} // namespace Panda
