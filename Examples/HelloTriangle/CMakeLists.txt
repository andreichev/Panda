project(HelloTriangle)

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

# HelloTriangle target

add_app(HelloTriangle "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_include_directories(HelloTriangle PRIVATE ${PROJECT_SOURCE_DIR}/src)

# set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set_target_properties(HelloTriangle
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIRECTORY}
    )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set_target_properties(HelloTriangle
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
)

set_target_properties(HelloTriangle
    PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${BIN_DIRECTORY}
)

# Dependencies - Foundation
target_link_libraries(HelloTriangle Foundation)
target_include_directories(HelloTriangle PRIVATE ${FOUNDATION_DIRECTORY}/include)

# Dependencies - Miren
target_link_libraries(HelloTriangle Miren)
target_include_directories(HelloTriangle PRIVATE ${MIREN_DIRECTORY}/include)

# Dependencies - Panda
target_link_libraries(HelloTriangle Panda)
target_include_directories(HelloTriangle PRIVATE ${PANDA_DIRECTORY}/src)