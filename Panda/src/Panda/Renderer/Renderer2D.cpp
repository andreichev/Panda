#include "Panda/Renderer/Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Panda {

struct Vertex {
    Vertex(glm::vec3 pos, glm::vec2 textureCoords, float textureIndex, Color color)
        : pos(pos)
        , textureCoords(textureCoords)
        , textureIndex(textureIndex)
        , color(color) {}

    Vertex()
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
    Miren::ShaderHandle shader;
    Foundation::Shared<Texture> whiteTexture;
    Miren::VertexLayoutHandle layout;
    Foundation::Shared<Texture> textures[MAX_TEXTURE_SLOTS];
    int samplers[MAX_TEXTURE_SLOTS];
    uint32_t textureSlotIndex;
    Vertex *vertices;
    uint32_t verticesCount;
    uint16_t *indices;
    uint32_t indicesCount;
    uint32_t vbSize;
    uint32_t ibSize;
};

static DrawCallData s_drawData;
Miren::ViewId Renderer2D::s_viewId = 0;
OrthographicCamera *Renderer2D::s_camera = nullptr;

void Renderer2D::init() {
    s_drawData.vbSize = 0;
    s_drawData.indicesCount = 0;
    s_drawData.vertices =
        (Vertex *)ALLOC(Foundation::getAllocator(), sizeof(Vertex) * MAX_VERTICES_COUNT);
    s_drawData.indices =
        (uint16_t *)ALLOC(Foundation::getAllocator(), sizeof(uint16_t) * MAX_INDICES_COUNT);
    Panda::ShaderAsset shaderAsset = Panda::AssetLoader::loadShader(
        "shaders/renderer2d/renderer2d_vertex.glsl", "shaders/renderer2d/renderer2d_fragment.glsl");
    s_drawData.shader = Miren::createShader(shaderAsset.vertexCode, shaderAsset.fragmentCode);
    Miren::VertexBufferLayoutData layoutData;
    // Position
    layoutData.pushVec3();
    // Texture coordinates
    layoutData.pushVec2();
    // Texture Index
    layoutData.pushFloat(1);
    // Color
    layoutData.pushVec4();
    s_drawData.layout = Miren::createVertexLayout(layoutData);
    s_drawData.textureSlotIndex = 1;
    uint32_t *whiteTextureData = (uint32_t *)ALLOC(Foundation::getAllocator(), sizeof(uint32_t));
    *whiteTextureData = 0xffffffff;
    s_drawData.whiteTexture = Foundation::makeShared<Texture>((uint8_t *)whiteTextureData, 1, 1);
    s_drawData.textures[0] = s_drawData.whiteTexture;
    for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        s_drawData.samplers[i] = i;
    }
    Miren::setUniform(
        s_drawData.shader, "u_textures", s_drawData.samplers, Miren::UniformDataType::IntArray);
}

void Renderer2D::terminate() {
    Miren::deleteShader(s_drawData.shader);
    Miren::deleteVertexLayout(s_drawData.layout);
    for (int i = 0; i < MAX_TEXTURE_SLOTS; ++i) {
        s_drawData.textures[i] = nullptr;
    }
    s_drawData.whiteTexture = nullptr;
    FREE(Foundation::getAllocator(), s_drawData.vertices);
    FREE(Foundation::getAllocator(), s_drawData.indices);
}

void Renderer2D::begin() {
    s_drawData.stats.quadCount = 0;
    s_drawData.stats.drawCalls = 0;
    s_drawData.verticesCount = 0;
    s_drawData.vbSize = 0;
    s_drawData.indicesCount = 0;
    s_drawData.ibSize = 0;
    s_drawData.textureSlotIndex = 1;
}

void Renderer2D::drawRect(RectData rect) {
    glm::mat4 identity = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(rect.origin.x, rect.origin.y, 0.f);
    glm::vec3 scale = glm::vec3(rect.size.width, rect.size.height, 1.f);
    glm::mat4 transform =
        glm::translate(identity, position) *
        glm::rotate(identity, glm::radians(rect.rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(identity, scale);
    drawRect(transform, rect);
}

void Renderer2D::drawRect(glm::mat4 &transform, RectData rect) {
    uint16_t indicesOffset = s_drawData.verticesCount;
    uint32_t &verticesCount = s_drawData.verticesCount;
    glm::vec4 positions[4];
    positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    glm::vec2 textureCoords[4];
    textureCoords[0] = {0.0f, 0.0f};
    textureCoords[1] = {1.0f, 0.0f};
    textureCoords[2] = {1.0f, 1.0f};
    textureCoords[3] = {0.0f, 1.0f};

    float textureIndex = 0.0f;
    if (rect.texture != nullptr) {
        for (uint32_t i = 1; i < s_drawData.textureSlotIndex; i++) {
            if (*s_drawData.textures[i] == *rect.texture) {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.0f) {
            // TODO: Check if next batch is needed
            textureIndex = (float)s_drawData.textureSlotIndex;
            s_drawData.textures[s_drawData.textureSlotIndex] = rect.texture;
            s_drawData.textureSlotIndex++;
        }
    }

    for (int i = 0; i < 4; i++) {
        s_drawData.vertices[verticesCount].pos = transform * positions[i];
        s_drawData.vertices[verticesCount].textureIndex = textureIndex;
        s_drawData.vertices[verticesCount].color = rect.color;
        s_drawData.vertices[verticesCount].textureCoords = textureCoords[i];
        verticesCount++;
    }
    uint32_t &indicesCount = s_drawData.indicesCount;
    s_drawData.indices[indicesCount++] = indicesOffset + 0;
    s_drawData.indices[indicesCount++] = indicesOffset + 1;
    s_drawData.indices[indicesCount++] = indicesOffset + 2;
    s_drawData.indices[indicesCount++] = indicesOffset + 2;
    s_drawData.indices[indicesCount++] = indicesOffset + 3;
    s_drawData.indices[indicesCount++] = indicesOffset + 0;

    s_drawData.vbSize += sizeof(Vertex) * 4;
    s_drawData.ibSize += sizeof(uint16_t) * 6;
    PND_ASSERT(verticesCount < MAX_VERTICES_COUNT, "VERTICES LIMIT REACHED");
    PND_ASSERT(indicesCount < MAX_INDICES_COUNT, "INDICES LIMIT REACHED");
    s_drawData.stats.quadCount += 1;
}

Renderer2D::Statistics Renderer2D::getStats() {
    return s_drawData.stats;
}

void Renderer2D::end() {
    if (s_drawData.verticesCount == 0 || s_camera == nullptr) {
        return;
    }
    Miren::setShader(s_drawData.shader);
    Miren::setUniform(s_drawData.shader,
        "projViewMtx",
        (void *)&s_camera->getViewProjectionMatrix(),
        Miren::UniformDataType::Mat4);

    Miren::TransientVertexBuffer tvb;
    Miren::allocTransientVertexBuffer(&tvb, s_drawData.vbSize);
    memcpy(tvb.data, s_drawData.vertices, s_drawData.vbSize);

    Miren::TransientIndexBuffer tib;
    Miren::allocTransientIndexBuffer(
        &tib, s_drawData.indicesCount, Miren::BufferElementType::UnsignedShort);
    memcpy(tib.data, s_drawData.indices, s_drawData.ibSize);

    Miren::setState(0);
    for (int i = 0; i < s_drawData.textureSlotIndex; i++) {
        Miren::setTexture(s_drawData.textures[i]->getHandle(), i);
    }
    Miren::setVertexLayout(s_drawData.layout);
    Miren::setVertexBuffer(tvb.handle, tvb.startVertex);
    Miren::setIndexBuffer(tib.handle, tib.startIndex, s_drawData.indicesCount);
    Miren::submit(s_viewId);
}

void Renderer2D::setViewId(Miren::ViewId id) {
    s_viewId = id;
}

void Renderer2D::setCamera(OrthographicCamera *camera) {
    s_camera = camera;
}

} // namespace Panda
