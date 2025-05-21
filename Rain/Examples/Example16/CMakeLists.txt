project(Example16)

set(CMAKE_CXX_STANDARD 20)

add_executable(Example16 main.cpp)

# Dependencies - Rain
target_link_libraries(Example16 Rain)