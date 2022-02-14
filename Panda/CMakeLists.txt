project(Panda)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES
        src/*.cpp
        src/*.mm
        )

file(GLOB_RECURSE INCLUDE
        src/*.h
        src/*.hpp
        )

# Panda target

add_library(Panda STATIC "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_precompile_headers(Panda PRIVATE src/pndpch.hpp)

target_include_directories(Panda PRIVATE ${PROJECT_SOURCE_DIR}/src)

set_target_properties(Panda
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
        )

# Dependencies - Panda
target_link_libraries(Panda spdlog)
target_include_directories(Panda PRIVATE Vendor/spdlog/include)

# Dependencies - GLFW
target_link_libraries(Panda glfw)
target_include_directories(Panda PRIVATE Vendor/glfw/include)

# Dependencies - GLM
target_link_libraries(Panda glm)
target_include_directories(Panda PRIVATE Vendor/glm)

# Dependencies - GLAD
target_link_libraries(Panda glad)
target_include_directories(Panda PRIVATE Vendor/glad/include)

# Dependencies - stb_image
target_link_libraries(Panda stb_image)
target_include_directories(Panda PRIVATE Vendor/stb_image)