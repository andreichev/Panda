project(MirenTools)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE src/*.hpp include/*.hpp)

# MirenTools target
add_library(MirenTools STATIC "${SOURCES}" "${INCLUDE}")

target_include_directories(MirenTools PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(MirenTools PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Dependencies - Foundation
target_link_libraries(MirenTools Foundation)
