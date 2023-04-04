project(HelloCanvas)

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

# HelloCanvas target

add_app(HelloCanvas "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_include_directories(HelloCanvas PRIVATE ${PROJECT_SOURCE_DIR}/src)

# set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set_target_properties(HelloCanvas
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIRECTORY}
    )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set_target_properties(HelloCanvas
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
)

set_target_properties(HelloCanvas
    PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${BIN_DIRECTORY}
)

# Dependencies - Panda
target_link_libraries(HelloCanvas Panda)