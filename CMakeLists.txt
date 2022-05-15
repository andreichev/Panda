cmake_minimum_required(VERSION 3.16)

project("PandaSandbox")

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

set(PANDA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Panda)
set(BIN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(Panda)
add_subdirectory(Sandbox)