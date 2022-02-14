project(imgui)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(imgui STATIC ${SOURCES} ${HEADERS})
target_include_directories(imgui PRIVATE include)

# Dependency - imgui
target_link_libraries(imgui glfw)
target_include_directories(imgui PRIVATE ../glfw/include)

# Dependency - glad
target_link_libraries(imgui glad)
target_include_directories(imgui PRIVATE ../glad/include)