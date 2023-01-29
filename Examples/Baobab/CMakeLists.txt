project(Baobab)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES
        src/*.cpp
        )

file(GLOB_RECURSE INCLUDE
        src/*.h
        src/*.hpp
        )

file(GLOB RESOURCE_FOLDERS
        resources/common/*
        )

if (DESKTOP)
    file(GLOB RESOURCE_DESKTOP_FOLDERS resources/desktop/*)
    list (APPEND RESOURCE_FOLDERS "${RESOURCE_DESKTOP_FOLDERS}")
elseif(PLATFORM_IOS)
    file(GLOB RESOURCE_IOS_FOLDERS resources/ios/*)
    list (APPEND RESOURCE_FOLDERS "${RESOURCE_IOS_FOLDERS}")
endif()

list(FILTER RESOURCE_FOLDERS EXCLUDE REGEX "\\.DS_Store")

# Baobab target

add_app(Baobab "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_precompile_headers(Baobab PRIVATE src/pch.hpp)
target_include_directories(Baobab PRIVATE ${PROJECT_SOURCE_DIR}/src)

# set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set_target_properties(Baobab
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIRECTORY}
    )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set_target_properties(Baobab
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
)

set_target_properties(Baobab
    PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${BIN_DIRECTORY}
)

# Dependencies - Foundation
target_link_libraries(Baobab Foundation)
target_include_directories(Baobab PRIVATE ${FOUNDATION_DIRECTORY}/include)

# Dependencies - Miren
target_link_libraries(Baobab Miren)
target_include_directories(Baobab PRIVATE ${MIREN_DIRECTORY}/include)

# Dependencies - Panda
target_link_libraries(Baobab Panda)
target_include_directories(Baobab PRIVATE ${PANDA_DIRECTORY}/src)