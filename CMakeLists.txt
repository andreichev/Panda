cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 14)

project("Panda")

add_subdirectory(Vendor/spdlog)
add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/glad)
add_subdirectory(Vendor/glm)
add_subdirectory(Vendor/stb_image)
add_subdirectory(Vendor/imgui)

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

set(FOUNDATION_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Foundation)
set(MIREN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Miren)
set(PANDA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Panda)
set(VENDOR_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)
set(BIN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include(cmake/add_app.cmake)

add_subdirectory(Foundation)
add_subdirectory(Miren)
add_subdirectory(Panda)
add_subdirectory(Sandbox)