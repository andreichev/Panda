project(TestProject)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDE src/*.hpp)

add_library(TestProject SHARED "${SOURCES}" TestProject.def "${INCLUDE}")

if(PLATFORM_IOS OR PLATFORM_MACOS)
    add_custom_command(TARGET TestProject POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TestProject> ${BIN_DIRECTORY}/PandaEditor.app/Contents/Resources
    )
else ()
    add_custom_command(TARGET TestProject POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TestProject> ${BIN_DIRECTORY}
    )
endif ()

target_include_directories(TestProject PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Dependencies - ScriptCore
target_link_libraries(TestProject ScriptCoreCpp)

