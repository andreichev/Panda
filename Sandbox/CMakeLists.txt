project(Sandbox)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES
        src/*.cpp
        )

file(GLOB_RECURSE INCLUDE
        src/*.h
        src/*.hpp
        )

file(GLOB RESOURCE_FOLDERS
        Resources-common/*
        )

if (DESKTOP)
    file(GLOB RESOURCE_DESKTOP_FOLDERS Resources-pc/*)
    list (APPEND RESOURCE_FOLDERS "${RESOURCE_DESKTOP_FOLDERS}")
elseif(PLATFORM_IOS)
    file(GLOB RESOURCE_IOS_FOLDERS Resources-ios/*)
    list (APPEND RESOURCE_FOLDERS "${RESOURCE_IOS_FOLDERS}")
endif()

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