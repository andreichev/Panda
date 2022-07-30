project(Panda)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPND_PROFILE)
endif ()

set(CMAKE_CXX_STANDARD 14)

include(cmake/add_app.cmake)

add_subdirectory(Vendor/spdlog)
add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/glad)
add_subdirectory(Vendor/glm)
add_subdirectory(Vendor/stb_image)
add_subdirectory(Vendor/imgui)

file(GLOB_RECURSE SOURCES src/Panda/*.cpp src/Platform/RendererImpl/OpenGL/*.cpp)
file(GLOB_RECURSE INCLUDE src/Panda/*.hpp src/Platform/RendererImpl/OpenGL/*.hpp)

# Entry point realization binding
if (DESKTOP)
    list(APPEND SOURCES "src/Platform/EntryPoint/Desktop/main.cpp")
elseif (PLATFORM_IOS)
    list(APPEND SOURCES "src/Platform/EntryPoint/iOS/main.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# Other platform specific realizations
if (DESKTOP)
    list(APPEND INCLUDE "src/Platform/WindowImpl/GlfwWindow/GlfwWindow.hpp")
    list(APPEND SOURCES "src/Platform/WindowImpl/GlfwWindow/GlfwWindow.cpp")
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/OpenGLContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/OpenGLContext.cpp")
elseif (PLATFORM_IOS)
    file(GLOB_RECURSE GLOB_RESULT_INCLUDE src/Platform/WindowImpl/UIKitWindow/*.hpp)
    file(GLOB_RECURSE GLOB_RESULT_SOURCES src/Platform/WindowImpl/UIKitWindow/*.cpp src/Platform/WindowImpl/UIKitWindow/*.mm)
    list(APPEND INCLUDE ${GLOB_RESULT_INCLUDE})
    list(APPEND SOURCES ${GLOB_RESULT_SOURCES})
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/GlesContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/GlesContext.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# Panda target

add_library(Panda STATIC "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_precompile_headers(Panda PRIVATE src/pndpch.hpp)

target_include_directories(Panda PRIVATE ${PROJECT_SOURCE_DIR}/src)

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

# Dependencies - Dear ImGui
target_link_libraries(Panda imgui)
target_include_directories(Panda PRIVATE Vendor/imgui)