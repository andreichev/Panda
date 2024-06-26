project(HelloCanvas)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
        src/*.cpp
        )

file(GLOB_RECURSE INCLUDE
        src/*.h
        src/*.hpp
        )

file(GLOB RESOURCE_FOLDERS
        resources/common/*
        ${PANDA_DIRECTORY}/resources/common/*
        )

if (DESKTOP)
    file(GLOB PANDA_DESKTOP_RESOURCES ${PANDA_DIRECTORY}/resources/desktop/*)
    list(APPEND RESOURCE_FOLDERS "${PANDA_DESKTOP_RESOURCES}")
    file(GLOB DESKTOP_RESOURCES resources/desktop/*)
    list(APPEND RESOURCE_FOLDERS "${DESKTOP_RESOURCES}")
elseif(PLATFORM_IOS)
    file(GLOB PANDA_IOS_RESOURCES ${PANDA_DIRECTORY}/resources/ios/*)
    list(APPEND RESOURCE_FOLDERS "${PANDA_IOS_RESOURCES}")
    file(GLOB IOS_RESOURCES resources/ios/*)
    list(APPEND RESOURCE_FOLDERS "${IOS_RESOURCES}")
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

# Dependencies - Panda
target_link_libraries(HelloCanvas PandaUI)