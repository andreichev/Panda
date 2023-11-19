project(PandaUI)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/PandaUI/*.cpp)
file(GLOB_RECURSE INCLUDE src/PandaUI/*.hpp include/PandaUI/*.hpp)

# PandaUI target

add_library(PandaUI STATIC "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

target_include_directories(PandaUI PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(PandaUI PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Dependencies - Miren
target_link_libraries(PandaUI Miren)

# Dependencies - Panda
target_link_libraries(PandaUI Panda)