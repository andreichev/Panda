#pragma once

#include "Panda/Base/Base.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

#define MAX_VERTICES_COUNT 3000
#define MAX_INDICES_COUNT 2000

struct RectData {
    RectData() {}

    Point origin;
    Size size;
    Color color;
};

struct OrthoProjData {
    Size screenSize;
};

class Renderer2D {
public:
    static void init();
    static void begin();
    static void drawRect(RectData rect);
    static void end();
    static void terminate();

private:
    struct Vertex {
        Point pos;
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