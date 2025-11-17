project(Panda)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-DPROFILE)
endif ()

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/Panda/*.cpp)
file(GLOB_RECURSE INCLUDE src/Panda/*.hpp include/Panda/*.hpp)

# Panda target

add_library(Panda STATIC "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_include_directories(Panda PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Panda PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Dependencies - Foundation
target_link_libraries(Panda Foundation)

# Dependencies - Rain
target_link_libraries(Panda Rain)

# Dependencies - Fern
target_link_libraries(Panda Fern)

# Dependencies - GLM
target_link_libraries(Panda glm)
target_include_directories(Panda PUBLIC ${VENDOR_DIRECTORY}/glm)

# Dependencies - stb_image
target_link_libraries(Panda stb_image)

# Dependencies - entt
target_link_libraries(Panda EnTT)
target_include_directories(Panda PUBLIC ${VENDOR_DIRECTORY}/entt/src)

# Dependencies - Dear ImGui
target_link_libraries(Panda imgui)
target_include_directories(Panda PUBLIC ${VENDOR_DIRECTORY}/imgui)

# Dependencies - ImGuizmo
target_link_libraries(Panda ImGuizmo)
target_include_directories(Panda PUBLIC ${VENDOR_DIRECTORY}/ImGuizmo)

# Dependencies - dylib
target_link_libraries(Panda dylib)

# Dependencies - box2d
target_link_libraries(Panda box2d)

# Dependencies - cgltf
target_link_libraries(Panda cgltf)