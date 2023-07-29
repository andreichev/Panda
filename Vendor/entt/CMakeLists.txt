project(entt)

set(SOURCES src/entt.cpp)
set(HEADERS include/entt/entt.hpp)

add_library(entt STATIC ${SOURCES} ${HEADERS})
target_include_directories(entt PRIVATE include)