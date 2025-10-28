project(Example17)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example17 main.cpp)

# Dependencies - Rain
target_link_libraries(Example17 Rain)