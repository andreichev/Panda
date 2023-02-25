#include "Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Panda {

Renderer2D::DrawData Renderer2D::s_drawData;

void Renderer2D::init() {
    s_drawData.vbSize = 0;
    s_drawData.indicesCount = 0;
    s_drawData.vertices = (Renderer2D::Vertex *)ALLOC(
        Foundation::getAllocator(), sizeof(Renderer2D::Vertex) * MAX_VERTICES_COUNT);
    s_drawData.indices =
        (uint16_t *)ALLOC(Foundation::getAllocator(), sizeof(uint16_t) * MAX_INDICES_COUNT);
    s_drawData.shader = Miren::createShader(
        "shaders/renderer2d/renderer2d_vertex.glsl", "shaders/renderer2d/renderer2d_fragment.glsl");
    Miren::VertexBufferLayoutData layoutData;
    // Position
    layoutData.pushVec3();
    // Color
    layoutData.pushVec4();
    s_drawData.layout = Miren::createVertexLayout(layoutData);
}

void Renderer2D::terminate() {
    Miren::deleteShader(s_drawData.shader);
    Miren::deleteVertexLayout(s_drawData.layout);
    FREE(Foundation::getAllocator(), s_drawData.vertices);
    FREE(Foundation::getAllocator(), s_drawData.indices);
}

void Renderer2D::begin() {
    s_drawData.verticesCount = 0;
    s_drawData.vbSize = 0;
    s_drawData.indicesCount = 0;
    s_drawData.ibSize = 0;
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
    for (glm::vec4 &pos : positions) {
        s_drawData.vertices[verticesCount].pos = transform * pos;
        s_drawData.vertices[verticesCount].color = rect.color;
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
}

void Renderer2D::end() {
    float L = 0;
    float R = 600;
    float T = 0;
    float B = 600;
    s_drawData.projMat = glm::ortho(L, R, B, T, -1.f, 1.f);
    Miren::setUniform(
        s_drawData.shader, "ProjMtx", &s_drawData.projMat, Miren::UniformDataType::Mat4);

    Miren::TransientVertexBuffer tvb;
    Miren::allocTransientVertexBuffer(&tvb, s_drawData.vbSize);
    memcpy(tvb.data, s_drawData.vertices, s_drawData.vbSize);

    Miren::TransientIndexBuffer tib;
    Miren::allocTransientIndexBuffer(
        &tib, s_drawData.indicesCount, Miren::BufferElementType::UnsignedShort);
    memcpy(tib.data, s_drawData.indices, s_drawData.ibSize);

    Miren::setState(0);
    Miren::setVertexLayout(s_drawData.layout);
    Miren::setVertexBuffer(tvb.handle, tvb.startVertex);
    Miren::setIndexBuffer(tib.handle, tib.startIndex, s_drawData.indicesCount);
    Miren::setShader(s_drawData.shader);
    Miren::submit();
}

} // namespace Panda