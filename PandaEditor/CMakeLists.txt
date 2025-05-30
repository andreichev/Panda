project(PandaEditor)

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
else()
    message(FATAL_ERROR "Only desktop platform supported for editor")
endif()

list(FILTER RESOURCE_FOLDERS EXCLUDE REGEX "\\.DS_Store")

# PandaEditor target

add_app(PandaEditor "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_include_directories(PandaEditor PRIVATE ${PROJECT_SOURCE_DIR}/src)

# set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set_target_properties(PandaEditor
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIRECTORY}
    )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set_target_properties(PandaEditor
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
)

set_target_properties(PandaEditor
    PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${BIN_DIRECTORY}
)

# Dependencies - Panda
target_link_libraries(PandaEditor Panda)

# Dependencies - PandaUI
target_link_libraries(PandaEditor PandaUI)

# Dependencies - nfd
add_subdirectory(Vendor/nfd)
set_target_properties(nfd PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(PandaEditor nfd)

# DEFINE EDITOR
target_compile_definitions(PandaEditor PUBLIC PND_EDITOR)
target_compile_definitions(Panda PUBLIC PND_EDITOR)