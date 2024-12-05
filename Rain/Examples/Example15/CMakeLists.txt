project(Example15)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example15 main.cpp)

# Dependencies - Rain
target_link_libraries(Example15 Rain)