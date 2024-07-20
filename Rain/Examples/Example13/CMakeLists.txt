project(Example13)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example13 main.cpp)

# Dependencies - Rain
target_link_libraries(Example13 Rain)