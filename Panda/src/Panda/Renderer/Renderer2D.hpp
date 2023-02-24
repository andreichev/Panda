#pragma once

#include "Panda/Base/Base.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

#define MAX_VERTICES_COUNT 3000
#define MAX_INDICES_COUNT 2000

struct RectData {
    RectData()
        : origin()
        , size()
        , color()
        , rotation(0) {}

    Point origin;
    Size size;
    Color color;
    float rotation;
};

class Renderer2D {
public:
    static void init();
    static void begin();
    static void drawRect(RectData rect);
    static void end();
    static void terminate();

private:
    static void drawRect(glm::mat4 &transform, RectData rect);

    struct Vertex {
        Vertex(glm::vec3 pos, Color color)
            : pos(pos)
            , color(color) {}

        Vertex()
            : pos()
            , color() {}
        glm::vec3 pos;
        Color color;
    };

    struct DrawData {
        glm::mat4 projMat;
        Miren::ShaderHandle shader;
        Miren::VertexLayoutHandle layout;
        Vertex *vertices;
        uint32_t verticesCount;
        uint16_t *indices;
        uint32_t indicesCount;
        uint32_t vbSize;
        uint32_t ibSize;
    };

    static DrawData s_drawData;
};

} // namespace Panda