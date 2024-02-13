#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/Renderer/Texture.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

#define MAX_VERTICES_COUNT 100000
#define MAX_INDICES_COUNT 160000
#define MAX_TEXTURE_SLOTS 8

/// Renderer2D отвечает за то, чтобы рисовать примитивные двумерные 2D фигуры.
/// Для этого Renderer2D создает шейдер и буферы с вершинами, текстурами, индексами.
/// Renderer2D вызывает отрисовку после вызова end();
/// Если требуется не стандартный framebuffer, его требуется создать вне Renderer2D
/// и передать viewId.
class Renderer2D {
public:
    struct RectData {
        RectData()
            : center()
            , size()
            , color()
            , texture(nullptr)
            , rotation(0) {}

        Vec3 center;
        Size size;
        Color color;
        float rotation;
        Foundation::Shared<Texture> texture;
    };

    struct Statistics {
        uint32_t drawCalls = 0;
        uint32_t quadCount = 0;

        uint32_t getTotalVertexCount() const {
            return quadCount * 4;
        }
        uint32_t getTotalIndexCount() const {
            return quadCount * 6;
        }
    };

    struct Vertex2D {
        Vertex2D(glm::vec3 pos, glm::vec2 textureCoords, float textureIndex, Color color)
            : pos(pos)
            , textureCoords(textureCoords)
            , textureIndex(textureIndex)
            , color(color) {}

        Vertex2D()
            : pos()
            , textureCoords()
            , textureIndex(0)
            , color() {}

        glm::vec3 pos;
        glm::vec2 textureCoords;
        float textureIndex;
        Color color;
    };

    struct DrawCallData {
        Renderer2D::Statistics stats;
        glm::mat4 projMat;
        Miren::ProgramHandle shader;
        Foundation::Shared<Texture> whiteTexture;
        Miren::VertexLayoutHandle layout;
        Foundation::Shared<Texture> textures[MAX_TEXTURE_SLOTS];
        int samplers[MAX_TEXTURE_SLOTS];
        uint32_t textureSlotIndex;
        Vertex2D *vertices;
        uint32_t verticesCount;
        uint16_t *indices;
        uint32_t indicesCount;
        uint32_t vbSize;
        uint32_t ibSize;
    };

    Renderer2D();
    ~Renderer2D();
    void begin();
    void drawRect(RectData rect);
    void end();
    Statistics getStats();
    void setViewId(Miren::ViewId id);
    void setViewProj(glm::mat4 viewProj);

private:
    glm::mat4 m_viewProj;
    Miren::ViewId m_viewId;
    DrawCallData m_drawData;
    void drawRect(glm::mat4 &transform, RectData rect);
};

} // namespace Panda