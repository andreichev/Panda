project(Miren)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES src/Miren/*.cpp)
file(GLOB_RECURSE INCLUDE src/Miren/*.hpp include/Miren/*.hpp)

file(GLOB_RECURSE GLOB_RESULT_INCLUDE src/Platform/RendererImpl/OpenGL/*.hpp)
file(GLOB_RECURSE GLOB_RESULT_SOURCES src/Platform/RendererImpl/OpenGL/*.cpp)
list(APPEND INCLUDE ${GLOB_RESULT_INCLUDE})
list(APPEND SOURCES ${GLOB_RESULT_SOURCES})

# Other platform specific realizations
if (DESKTOP)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/OpenGLContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/OpenGLContext.cpp")
elseif (PLATFORM_IOS)
    list(APPEND INCLUDE "src/Platform/RendererImpl/Context/GlesContext.hpp")
    list(APPEND SOURCES "src/Platform/RendererImpl/Context/GlesContext.mm")
else ()
    message(FATAL_ERROR "Unknown platform")
endif ()

# Miren target

add_library(Miren STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(Miren PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Miren PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (DESKTOP)
    # Dependencies - GLFW, only for Desktop :)
    target_link_libraries(Miren glfw)
    target_include_directories(Miren PRIVATE Vendor/glfw/include)
elseif (PLATFORM_IOS)
    # add_definitions("-x objective-c++")
    # Locate system libraries on iOS
    find_library(OPENGLES OpenGLES)
    find_library(METALKIT_LIBRARY MetalKit)
    find_library(QUARTZ_CORE QuartzCore)
    find_library(FOUNDATION Foundation)
    find_library(MOBILECORESERVICES MobileCoreServices)
    find_library(CFNETWORK CFNetwork)
    find_library(SYSTEMCONFIGURATION SystemConfiguration)

    target_link_libraries(Miren
            ${UIKIT} ${METALKIT_LIBRARY} ${FOUNDATION} ${CFNETWORK} ${SYSTEMCONFIGURATION} ${QUARTZ_CORE} ${OPENGLES}
    )
endif ()

# Dependencies - Foundation
target_link_libraries(Miren Foundation)
target_include_directories(Miren PUBLIC ${FOUNDATION_DIRECTORY}/include)

# Dependencies - GLM
target_link_libraries(Miren glm)
target_include_directories(Miren PRIVATE ${VENDOR_DIRECTORY}/glm)

# Dependencies - GLAD
target_link_libraries(Miren glad)
target_include_directories(Miren PRIVATE ${VENDOR_DIRECTORY}/glad/include)

# Dependencies - stb_image
target_link_libraries(Miren stb_image)
target_include_directories(Miren PRIVATE ${VENDOR_DIRECTORY}/stb_image)