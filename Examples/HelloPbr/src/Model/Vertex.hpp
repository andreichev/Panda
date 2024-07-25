//
// Created by Michael Andreichev on 22.09.2023.
//

#include <glm/glm.hpp>
#include <Miren/Miren.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec4 color;
    float light;

    Vertex()
        : pos()
        , uv()
        , color()
        , light() {}

    Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 normal, float light)
        : pos(pos)
        , uv(uv)
        , normal(normal)
        , color(1.f, 1.f, 1.f, 1.f)
        , light(light) {}

    Vertex(
        float x,
        float y,
        float z,
        float u,
        float v,
        uint32_t rgba,
        float nx,
        float ny,
        float nz,
        float light
    )
        : pos(x, y, z)
        , uv(u, v)
        , normal(nx, ny, nz)
        , color()
        , light(light) {
        uint8_t r = rgba >> 24;
        uint8_t g = rgba >> 16;
        uint8_t b = rgba >> 8;
        uint8_t a = rgba >> 0;
        color.r = (r) / 255.f;
        color.g = (g) / 255.f;
        color.b = (b) / 255.f;
        color.a = (a) / 255.f;
    }

    inline static Miren::VertexBufferLayoutData createBufferLayout() {
        Miren::VertexBufferLayoutData data;
        // pos
        data.pushVec3();
        // uv
        data.pushVec2();
        // normal
        data.pushVec3();
        // color
        data.pushVec4();
        // light
        data.pushFloat(1);
        return data;
    }
};