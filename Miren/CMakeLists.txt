project(Miren)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/Miren/*.cpp)
file(GLOB_RECURSE INCLUDE src/Miren/*.hpp include/Miren/*.hpp)

file(GLOB_RECURSE GLOB_RESULT_INCLUDE src/Platform/RendererImpl/OpenGL/*.hpp)
file(GLOB_RECURSE GLOB_RESULT_SOURCES src/Platform/RendererImpl/OpenGL/*.cpp)
list(APPEND INCLUDE ${GLOB_RESULT_INCLUDE})
list(APPEND SOURCES ${GLOB_RESULT_SOURCES})

# Miren target

add_library(Miren STATIC "${SOURCES}" "${INCLUDE}")

target_compile_definitions(Miren PUBLIC GL_SILENCE_DEPRECATION)
target_compile_definitions(Miren PUBLIC GLES_SILENCE_DEPRECATION)
target_include_directories(Miren PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Miren PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (PLATFORM_WIN32 OR PLATFORM_LINUX)
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

# Dependencies - Fern
target_link_libraries(Miren Fern)

# Dependencies - GLM
target_link_libraries(Miren glm)
target_include_directories(Miren PRIVATE ${VENDOR_DIRECTORY}/glm)

if (DESKTOP)
    # Dependencies - GLAD
    target_link_libraries(Miren glad)
    target_include_directories(Miren PUBLIC ${VENDOR_DIRECTORY}/glad/include)
endif ()

# Dependencies - stb_image
target_link_libraries(Miren stb_image)
target_include_directories(Miren PRIVATE ${VENDOR_DIRECTORY}/stb_image)