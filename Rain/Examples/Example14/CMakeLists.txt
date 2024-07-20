project(Example14)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example14 main.cpp)

# Dependencies - Rain
target_link_libraries(Example14 Rain)