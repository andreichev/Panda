project(Sandbox)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES
        src/*.cpp
        src/*.mm
        )

file(GLOB_RECURSE INCLUDE
        src/*.h
        src/*.hpp
        )

file(GLOB RESOURCE_FOLDERS
        Resources-common/*
        Resources-pc/*
        )
list(FILTER RESOURCE_FOLDERS EXCLUDE REGEX "\\.DS_Store")

# Sandbox target

add_app(Sandbox "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_precompile_headers(Sandbox PRIVATE src/sndbxpch.hpp)
target_include_directories(Sandbox PRIVATE ${PROJECT_SOURCE_DIR}/src)

set_target_properties(Sandbox
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
        )

# Dependencies - Panda
target_link_libraries(Sandbox Panda)
target_include_directories(Sandbox PRIVATE ${PANDA_DIRECTORY}/src)