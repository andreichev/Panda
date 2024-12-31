cmake_minimum_required(VERSION 3.16)

project(ScriptCoreCpp)

set(CMAKE_CXX_STANDARD 20)
set(SCRIPT_CORE_BIN_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE include/*.hpp)

add_library(ScriptCoreCpp STATIC "${SOURCES}" ScriptCoreCpp.def "${INCLUDE}")

set_target_properties(ScriptCoreCpp
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${SCRIPT_CORE_BIN_DIRECTORY}
)

target_include_directories(ScriptCoreCpp PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(ScriptCoreCpp PUBLIC ${PROJECT_SOURCE_DIR}/include)

