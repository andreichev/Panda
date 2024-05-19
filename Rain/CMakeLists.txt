cmake_minimum_required(VERSION 3.27)
project(Rain)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE src/*.hpp include/Rain/*.hpp)

# Rain target

add_library(Rain STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(Rain PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Rain PUBLIC ${PROJECT_SOURCE_DIR}/include)
