project(glad)

set(SOURCES src/glad.c)
set(HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h)

add_library(glad STATIC ${SOURCES} ${HEADERS})
target_include_directories(glad PRIVATE include)