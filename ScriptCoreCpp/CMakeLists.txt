project(ScriptCoreCpp)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE include/*.hpp)

add_library(ScriptCoreCpp STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(ScriptCoreCpp PUBLIC ${PROJECT_SOURCE_DIR}/include)

