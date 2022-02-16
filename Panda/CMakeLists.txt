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
if(PLATFORM_MACOS)
    list (APPEND SOURCES "src/PlatformSpecificRealizations/EntryPoint/MacOS/main.cpp")
elseif(PLATFORM_IOS)
    list (APPEND SOURCES "src/PlatformSpecificRealizations/EntryPoint/iOS/main.m")
else()
    message(FATAL_ERROR "Unknown platform")
endif()

# Other platform specific realizations
if(DESKTOP)
    list (APPEND SOURCES "src/PlatformSpecificRealizations/WindowImpl/GlfwWindow/GlfwWindow.cpp")
    list (APPEND INCLUDE "src/PlatformSpecificRealizations/WindowImpl/GlfwWindow/GlfwWindow.hpp")
    list (APPEND SOURCES "src/PlatformSpecificRealizations/EventsImpl/EventsGlfw/EventsGlfw.cpp")
    list (APPEND INCLUDE "src/PlatformSpecificRealizations/EventsImpl/EventsGlfw/EventsGlfw.hpp")
endif()

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
endif()

# Dependencies - GLM
target_link_libraries(Panda glm)
target_include_directories(Panda PRIVATE Vendor/glm)

# Dependencies - GLAD
target_link_libraries(Panda glad)
target_include_directories(Panda PRIVATE Vendor/glad/include)

# Dependencies - stb_image
target_link_libraries(Panda stb_image)
target_include_directories(Panda PRIVATE Vendor/stb_image)