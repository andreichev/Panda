#include "PandaUI/UIRenderer.hpp"

#include <Panda/Assets/Base/AssetImporterBase.hpp>

#include "Panda/Assets/StaticResources.hpp"

namespace PandaUI {

UIRenderer::UIRenderer()
    : m_viewId(0)
    , m_viewProj(1.f)
    , m_drawData()
    , m_uiShader() {
    m_drawData.vbSize = 0;
    m_drawData.indicesCount = 0;
    m_drawData.vertices =
        (Vertex2D *)F_ALLOC(Foundation::getAllocator(), sizeof(Vertex2D) * MAX_VERTICES_COUNT);
    m_drawData.indices =
        (uint16_t *)F_ALLOC(Foundation::getAllocator(), sizeof(uint16_t) * MAX_INDICES_COUNT);
    Foundation::Memory vertexMem;
    Foundation::Memory fragmentMem;
    vertexMem = Panda::AssetImporterBase::loadData("panda-ui/ui_vertex.glsl");
    fragmentMem = Panda::AssetImporterBase::loadData("panda-ui/ui_fragment.glsl");
    m_uiShader = Miren::createProgram({vertexMem, fragmentMem});
    Miren::VertexBufferLayoutData layoutData;
    // Position
    layoutData.pushVec3();
    // Texture coordinates
    layoutData.pushVec2();
    // Color
    layoutData.pushVec4();
    m_drawData.layout = Miren::createVertexLayout(layoutData);
    reset();
}

UIRenderer::~UIRenderer() {
    if (m_uiShader.isValid()) { Miren::deleteProgram(m_uiShader); }
    if (m_drawData.layout.isValid()) { Miren::deleteVertexLayout(m_drawData.layout); }
    if (m_drawData.vertices) { F_FREE(Foundation::getAllocator(), m_drawData.vertices); }
    if (m_drawData.indices) { F_FREE(Foundation::getAllocator(), m_drawData.indices); }
}

void UIRenderer::begin(Miren::ViewId viewId) {
    reset();
    m_viewId = viewId;
}

void UIRenderer::drawRect(RectData rect) {
    glm::mat4 identity = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(rect.center.x, rect.center.y, rect.center.z);
    glm::vec3 scale = glm::vec3(rect.size.width, rect.size.height, 1.f);
    glm::mat4 transform =
        glm::translate(identity, position) *
        glm::rotate(identity, glm::radians(rect.rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(identity, scale);
    drawRect(transform, rect);
}

void UIRenderer::drawRect(glm::mat4 &transform, RectData rect) {
    if (!rect.texture) { rect.texture = Panda::StaticResources::whiteTexture; }
    if (rect.texture != m_drawData.texture) {
        flush();
        m_drawData.texture = rect.texture;
    }
    // TODO: Check if next batch is needed
    uint16_t indicesOffset = m_drawData.verticesCount;
    uint32_t &verticesCount = m_drawData.verticesCount;
    glm::vec4 positions[4];
    positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    glm::vec2 textureCoords[4];
    textureCoords[0] = {
        rect.textureCoords.origin.x, rect.textureCoords.origin.y + rect.textureCoords.size.height
    };
    textureCoords[1] = {
        rect.textureCoords.origin.x + rect.textureCoords.size.width,
        rect.textureCoords.origin.y + rect.textureCoords.size.height
    };
    textureCoords[2] = {
        rect.textureCoords.origin.x + rect.textureCoords.size.width, rect.textureCoords.origin.y
    };
    textureCoords[3] = {rect.textureCoords.origin.x, rect.textureCoords.origin.y};

    for (int i = 0; i < 4; i++) {
        m_drawData.vertices[verticesCount].pos = rect.transform * transform * positions[i];
        m_drawData.vertices[verticesCount].textureCoords = textureCoords[i];
        m_drawData.vertices[verticesCount].color = rect.color;
        verticesCount++;
    }
    uint32_t &indicesCount = m_drawData.indicesCount;
    m_drawData.indices[indicesCount++] = indicesOffset + 0;
    m_drawData.indices[indicesCount++] = indicesOffset + 1;
    m_drawData.indices[indicesCount++] = indicesOffset + 2;
    m_drawData.indices[indicesCount++] = indicesOffset + 2;
    m_drawData.indices[indicesCount++] = indicesOffset + 3;
    m_drawData.indices[indicesCount++] = indicesOffset + 0;

    m_drawData.vbSize += sizeof(Vertex2D) * 4;
    m_drawData.ibSize += sizeof(uint16_t) * 6;
    PND_ASSERT(verticesCount < MAX_VERTICES_COUNT, "VERTICES LIMIT REACHED");
    PND_ASSERT(indicesCount < MAX_INDICES_COUNT, "INDICES LIMIT REACHED");
    m_drawData.stats.quadCount += 1;
}

UIRenderer::Statistics UIRenderer::getStats() {
    return m_drawData.stats;
}

void UIRenderer::end() {
    flush();
}

void UIRenderer::reset() {
    m_drawData.stats.quadCount = 0;
    m_drawData.stats.drawCalls = 0;
    m_drawData.verticesCount = 0;
    m_drawData.vbSize = 0;
    m_drawData.indicesCount = 0;
    m_drawData.ibSize = 0;
}

void UIRenderer::flush() {
    if (m_drawData.verticesCount == 0) { return; }
    Miren::setShader(m_uiShader);
    Miren::TransientVertexBuffer tvb;
    Miren::allocTransientVertexBuffer(&tvb, m_drawData.vbSize);
    memcpy(tvb.data, m_drawData.vertices, m_drawData.vbSize);

    Miren::TransientIndexBuffer tib;
    Miren::allocTransientIndexBuffer(
        &tib, m_drawData.indicesCount, Miren::BufferElementType::UnsignedShort
    );
    memcpy(tib.data, m_drawData.indices, m_drawData.ibSize);
    Miren::setState(MIREN_STATE_DEPTH_TEST);
    Miren::TextureHandle texture = m_drawData.texture->getMirenHandle();
    Miren::addInputUniformBuffer(m_uiShader, "UBO", (void *)&m_viewProj, sizeof(glm::mat4));
    Miren::addInputTexture(m_uiShader, "u_texture", texture);
    Miren::setVertexLayout(m_drawData.layout);
    Miren::setVertexBuffer(tvb.handle, tvb.startVertex);
    Miren::setIndexBuffer(tib.handle, tib.startIndex, m_drawData.indicesCount);
    Miren::submit(m_viewId);
    reset();
}

void UIRenderer::setViewProj(glm::mat4 viewProj) {
    m_viewProj = viewProj;
}

} // namespace PandaUI
