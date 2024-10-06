project(ImGuizmo)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(ImGuizmo STATIC ${SOURCES} ${HEADERS})
target_include_directories(ImGuizmo PRIVATE include)

# Dependencies - Dear ImGui
target_link_libraries(ImGuizmo imgui)
target_include_directories(ImGuizmo PUBLIC ${VENDOR_DIRECTORY}/imgui)
