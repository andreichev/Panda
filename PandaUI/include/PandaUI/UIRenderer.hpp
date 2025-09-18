#pragma once

#include "Base.hpp"

#include <Panda/Assets/Base/AssetRef.hpp>
#include <Panda/Assets/TextureAsset.hpp>
#include <Miren/Miren.hpp>

namespace PandaUI {

#define MAX_VERTICES_COUNT 100000
#define MAX_INDICES_COUNT 160000
#define MAX_TEXTURE_SLOTS 8

/// UIRenderer отвечает за то, чтобы рисовать примитивные двумерные фигуры.
/// Для этого UIRenderer создает шейдер и буферы с вершинами, текстурами, индексами.
/// UIRenderer вызывает отрисовку после вызова flush();
/// Если требуется не стандартный framebuffer, его требуется создать вне UIRenderer
/// и передать Miren::ViewId.
class UIRenderer final {
public:
    struct RectData {
        RectData()
            : center()
            , size()
            , color()
            , texture()
            , rotation(0)
            , textureCoords(0, 0, 1, 1)
            , transform(1.f) {}

        glm::vec3 center;
        Rect textureCoords;
        Size size;
        Color color;
        float rotation;
        glm::mat4 transform;
        Panda::AssetRef<Panda::TextureAsset> texture;
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
        Vertex2D(
            glm::vec3 pos, glm::vec2 textureCoords, float textureIndex, bool isSelected, Color color
        )
            : pos(pos)
            , textureCoords(textureCoords)
            , color(color) {}

        Vertex2D()
            : pos()
            , textureCoords()
            , color() {}

        glm::vec3 pos;
        glm::vec2 textureCoords;
        glm::vec4 color;
    };

    struct DrawCallData {
        UIRenderer::Statistics stats;
        glm::mat4 projMat;
        Miren::VertexLayoutHandle layout;
        Panda::AssetRef<Panda::TextureAsset> texture;
        Vertex2D *vertices;
        uint32_t verticesCount;
        uint16_t *indices;
        uint32_t indicesCount;
        uint32_t vbSize;
        uint32_t ibSize;
    };

    UIRenderer();
    UIRenderer(UIRenderer &other) = delete;
    ~UIRenderer();
    void begin(Miren::ViewId id);
    void drawRect(RectData rect);
    void end();
    Statistics getStats();
    void setViewProj(glm::mat4 viewProj);
    UIRenderer &operator=(UIRenderer &other) = delete;

private:
    glm::mat4 m_viewProj;
    Miren::ViewId m_viewId;
    DrawCallData m_drawData;
    Miren::ProgramHandle m_uiShader;

    void reset();
    void flush();
    void drawRect(glm::mat4 &transform, RectData rect);
};

} // namespace PandaUI
