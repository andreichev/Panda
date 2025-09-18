#include "Panda/Renderer/Renderer2D.hpp"
#include "Panda/Assets/Base/AssetImporterBase.hpp"
#include "Panda/Assets/StaticResources.hpp"
#include "Panda/Renderer/Std140Buffer.hpp"

namespace Panda {

Renderer2D::Renderer2D()
    : m_viewId(0)
    , m_viewProj(1.f)
    , m_drawData()
    , m_mode(Mode::DEFAULT) {
    m_drawData.vbSize = 0;
    m_drawData.indicesCount = 0;
    m_drawData.vertices =
        (Vertex2D *)F_ALLOC(Foundation::getAllocator(), sizeof(Vertex2D) * MAX_VERTICES_COUNT);
    m_drawData.indices =
        (uint16_t *)F_ALLOC(Foundation::getAllocator(), sizeof(uint16_t) * MAX_INDICES_COUNT);
    Miren::VertexBufferLayoutData layoutData;
    // Position
    layoutData.pushVec3();
    // Texture coordinates
    layoutData.pushVec2();
    // Color
    layoutData.pushVec4();
    // Object id
    layoutData.pushUInt(1);
    m_drawData.layout = Miren::createVertexLayout(layoutData);
    reset();
}

Renderer2D::~Renderer2D() {
    if (m_drawData.layout.isValid()) { Miren::deleteVertexLayout(m_drawData.layout); }
    if (m_drawData.vertices) { F_FREE(Foundation::getAllocator(), m_drawData.vertices); }
    if (m_drawData.indices) { F_FREE(Foundation::getAllocator(), m_drawData.indices); }
}

void Renderer2D::begin(Mode mode, Miren::ViewId viewId) {
    m_mode = mode;
    reset();
    m_viewId = viewId;
}

void Renderer2D::drawRect(RectData rect) {
    glm::mat4 identity = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(rect.center.x, rect.center.y, rect.center.z);
    glm::vec3 scale = glm::vec3(rect.size.width, rect.size.height, 1.f);
    glm::mat4 transform =
        glm::translate(identity, position) *
        glm::rotate(identity, glm::radians(rect.rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(identity, scale);
    drawRect(transform, rect);
}

void Renderer2D::drawRect(glm::mat4 &transform, RectData rect) {
    if (!rect.material) { rect.material = StaticResources::defaultMaterial; }
    if (rect.material != m_drawData.material) {
        flush();
        m_drawData.material = rect.material;
    }
    if (!m_drawData.material) {
        PND_ASSERT(false, "Invalid material for rect");
        return;
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
        m_drawData.vertices[verticesCount].id = rect.id;
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

Renderer2D::Statistics Renderer2D::getStats() {
    return m_drawData.stats;
}

void Renderer2D::end() {
    flush();
}

void Renderer2D::reset() {
    m_drawData.stats.quadCount = 0;
    m_drawData.stats.drawCalls = 0;
    m_drawData.verticesCount = 0;
    m_drawData.vbSize = 0;
    m_drawData.indicesCount = 0;
    m_drawData.ibSize = 0;
}

void Renderer2D::flush() {
    if (m_drawData.verticesCount == 0) { return; }
    AssetRef<MaterialAsset> material;
    switch (m_mode) {
        case Mode::DEFAULT: {
            Miren::setState(MIREN_STATE_DEPTH_TEST);
            material = m_drawData.material;
            break;
        }
        case Mode::GEOMETRY_ONLY: {
            Miren::setState(MIREN_STATE_WIREFRAME);
            material = StaticResources::selectedGeometryMaterial;
            break;
        }
    }
    Miren::ProgramHandle shaderHandle = material->getShaderAsset()->getMirenHandle();
    Miren::setShader(shaderHandle);
    Std140Buffer uboVert;
    // projViewMtx
    uboVert.addMat4(&m_viewProj[0][0]);
    // modelMtx
    glm::mat4 model(1.f);
    uboVert.addMat4(&model[0][0]);
    Miren::addInputUniformBuffer(shaderHandle, "UBO_VERT", uboVert.getData(), uboVert.getSize());

    Miren::TransientVertexBuffer tvb;
    Miren::allocTransientVertexBuffer(&tvb, m_drawData.vbSize);
    memcpy(tvb.data, m_drawData.vertices, m_drawData.vbSize);

    Miren::TransientIndexBuffer tib;
    Miren::allocTransientIndexBuffer(
        &tib, m_drawData.indicesCount, Miren::BufferElementType::UnsignedShort
    );
    memcpy(tib.data, m_drawData.indices, m_drawData.ibSize);

    material->bindFields();
    Miren::setVertexLayout(m_drawData.layout);
    Miren::setVertexBuffer(tvb.handle, tvb.startVertex);
    Miren::setIndexBuffer(tib.handle, tib.startIndex, m_drawData.indicesCount);
    Miren::submit(m_viewId);
    reset();
}

void Renderer2D::setViewProj(glm::mat4 viewProj) {
    m_viewProj = viewProj;
}

} // namespace Panda
