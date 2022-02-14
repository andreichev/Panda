cmake_minimum_required(VERSION 3.16)

project("Panda")

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

set(PANDA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Panda)
set(BIN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPND_PROFILE)
endif ()

add_subdirectory(Panda)
add_subdirectory(Panda/Vendor/spdlog)
add_subdirectory(Panda/Vendor/glfw)
add_subdirectory(Panda/Vendor/glad)
add_subdirectory(Panda/Vendor/glm)
add_subdirectory(Panda/Vendor/stb_image)
add_subdirectory(Sandbox)