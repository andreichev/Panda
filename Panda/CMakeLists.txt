project(Panda)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES
        src/Panda/*.cpp
        )

file(GLOB_RECURSE INCLUDE
        src/Panda/*.h
        src/Panda/*.hpp
        )

# Entry point realization binding
if (PLATFORM_MACOS)
    list(APPEND SOURCES "src/PlatformSpecificRealizations/EntryPoint/MacOS/main.cpp")
elseif (PLATFORM_IOS)
    list(APPEND SOURCES "src/PlatformSpecificRealizations/EntryPoint/iOS/main.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# Other platform specific realizations
if (DESKTOP)
    list(APPEND INCLUDE "src/PlatformSpecificRealizations/WindowImpl/GlfwWindow/GlfwWindow.hpp")
    list(APPEND SOURCES "src/PlatformSpecificRealizations/WindowImpl/GlfwWindow/GlfwWindow.cpp")
    list(APPEND INCLUDE "src/PlatformSpecificRealizations/EventsImpl/EventsGlfw/EventsGlfw.hpp")
    list(APPEND SOURCES "src/PlatformSpecificRealizations/EventsImpl/EventsGlfw/EventsGlfw.cpp")
elseif (PLATFORM_IOS)
    list(APPEND INCLUDE "src/PlatformSpecificRealizations/WindowImpl/UIKitWindow/UIKitWindow.hpp")
    list(APPEND SOURCES "src/PlatformSpecificRealizations/WindowImpl/UIKitWindow/UIKitWindow.mm")
    list(APPEND INCLUDE "src/PlatformSpecificRealizations/WindowImpl/UIKitWindow/Controller/WonderController.hpp")
    list(APPEND SOURCES "src/PlatformSpecificRealizations/WindowImpl/UIKitWindow/Controller/WonderController.mm")
    list(APPEND INCLUDE "src/PlatformSpecificRealizations/WindowImpl/UIKitWindow/Controller/View/WonderView.hpp")
    list(APPEND SOURCES "src/PlatformSpecificRealizations/WindowImpl/UIKitWindow/Controller/View/WonderView.mm")
    list(APPEND INCLUDE "src/PlatformSpecificRealizations/EventsImpl/UIKitEvents/UIKitEvents.hpp")
    list(APPEND SOURCES "src/PlatformSpecificRealizations/EventsImpl/UIKitEvents/UIKitEvents.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# Panda target

add_library(Panda STATIC "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_precompile_headers(Panda PRIVATE src/pndpch.hpp)

target_include_directories(Panda PRIVATE ${PROJECT_SOURCE_DIR}/src)

set_target_properties(Panda
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
        )

# Dependencies - Panda
target_link_libraries(Panda spdlog)
target_include_directories(Panda PRIVATE Vendor/spdlog/include)

if (DESKTOP)
    # Dependencies - GLFW, only for Desktop :)
    target_link_libraries(Panda glfw)
    target_include_directories(Panda PRIVATE Vendor/glfw/include)
elseif (PLATFORM_IOS)
    # add_definitions("-x objective-c++")
    # Locate system libraries on iOS
    find_library(UIKIT UIKit)
    find_library(OPENGLES OpenGLES)
    find_library(METALKIT_LIBRARY MetalKit)
    find_library(QUARTZ_CORE QuartzCore)
    find_library(FOUNDATION Foundation)
    find_library(MOBILECORESERVICES MobileCoreServices)
    find_library(CFNETWORK CFNetwork)
    find_library(SYSTEMCONFIGURATION SystemConfiguration)

    target_link_libraries(Panda
            ${UIKIT} ${METALKIT_LIBRARY} ${FOUNDATION} ${CFNETWORK} ${SYSTEMCONFIGURATION} ${QUARTZ_CORE} ${OPENGLES}
    )
#    set_target_properties(
#            Panda PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "2"
#    )
endif ()

# Dependencies - GLM
target_link_libraries(Panda glm)
target_include_directories(Panda PRIVATE Vendor/glm)

# Dependencies - GLAD
target_link_libraries(Panda glad)
target_include_directories(Panda PRIVATE Vendor/glad/include)

# Dependencies - stb_image
target_link_libraries(Panda stb_image)
target_include_directories(Panda PRIVATE Vendor/stb_image)