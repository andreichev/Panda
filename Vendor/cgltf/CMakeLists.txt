project(cgltf)

set(SOURCES cgltf.cpp)
set(HEADERS cgltf.h)

add_library(cgltf STATIC ${SOURCES} ${HEADERS})

target_include_directories(cgltf PUBLIC .)