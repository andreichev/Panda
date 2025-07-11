#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/Assets/TextureAsset.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

#define MAX_VERTICES_COUNT 100000
#define MAX_INDICES_COUNT 160000
#define MAX_TEXTURE_SLOTS 8

/// Renderer2D отвечает за то, чтобы рисовать примитивные двумерные фигуры.
/// Для этого Renderer2D создает шейдер и буферы с вершинами, текстурами, индексами.
/// Renderer2D вызывает отрисовку после вызова end();
/// Если требуется не стандартный framebuffer, его требуется создать вне Renderer2D
/// и передать viewId.
class Renderer2D {
public:
    enum class Mode { DEFAULT, GEOMETRY_ONLY };

    struct RectData {
        RectData()
            : center()
            , size()
            , color()
            , texture(nullptr)
            , rotation(0)
            , id(-1)
            , textureCoords(0, 0, 1, 1)
            , transform(1.f) {}

        Vec3 center;
        Rect textureCoords;
        Size size;
        Color color;
        uint32_t id;
        float rotation;
        glm::mat4 transform;
        Foundation::Shared<Asset> texture;
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
            glm::vec3 pos,
            glm::vec2 textureCoords,
            float textureIndex,
            int32_t id,
            bool isSelected,
            Color color
        )
            : pos(pos)
            , textureCoords(textureCoords)
            , textureIndex(textureIndex)
            , color(color)
            , id(id) {}

        Vertex2D()
            : pos()
            , textureCoords()
            , textureIndex(0)
            , color()
            , id(-1) {}

        glm::vec3 pos;
        glm::vec2 textureCoords;
        int32_t textureIndex;
        glm::vec4 color;
        uint32_t id;
    };

    struct DrawCallData {
        Renderer2D::Statistics stats;
        glm::mat4 projMat;
        Miren::ProgramHandle shader;
        Foundation::Shared<TextureAsset> whiteTexture;
        Miren::VertexLayoutHandle layout;
        Foundation::Shared<TextureAsset> textures[MAX_TEXTURE_SLOTS];
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
    Renderer2D(Renderer2D &other) = delete;
    ~Renderer2D();
    void begin(Mode mode, Miren::ViewId id);
    void drawRect(RectData rect);
    void end();
    Statistics getStats();
    void setViewProj(glm::mat4 viewProj);
    Renderer2D &operator=(Renderer2D &other) = delete;

private:
    glm::mat4 m_viewProj;
    Mode m_mode;
    Miren::ViewId m_viewId;
    DrawCallData m_drawData;
    Miren::ProgramHandle m_defaultShader;
    Miren::ProgramHandle m_selectedGeometryShader;

    void reset();
    void flush();
    void drawRect(glm::mat4 &transform, RectData rect);
};

} // namespace Panda
