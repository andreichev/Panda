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
    layoutData.pushVec2();
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
    glm::mat4 transform = glm::translate(identity, position) * glm::scale(identity, scale);
    drawRect(transform, rect);
}

void Renderer2D::drawRect(glm::mat4 &transform, RectData rect) {
    uint32_t &verticesCount = s_drawData.verticesCount;
    Vertex vertices[] = {Vertex(Point(0, 0), rect.color),
        Vertex(Point(0, 1), rect.color),
        Vertex(Point(1, 1), rect.color),
        Vertex(Point(1, 0), rect.color)};
    for (Vertex &v : vertices) {
        glm::vec4 pos = transform * glm::vec4(v.pos.x, v.pos.y, 0.f, 1.f);
        s_drawData.vertices[verticesCount].pos = Point(pos.x, pos.y);
        s_drawData.vertices[verticesCount].color = v.color;
        verticesCount++;
    }

    uint32_t &indicesCount = s_drawData.indicesCount;
    s_drawData.indices[indicesCount++] = 0;
    s_drawData.indices[indicesCount++] = 2;
    s_drawData.indices[indicesCount++] = 1;
    s_drawData.indices[indicesCount++] = 0;
    s_drawData.indices[indicesCount++] = 3;
    s_drawData.indices[indicesCount++] = 2;

    s_drawData.vbSize += sizeof(Vertex) * 4;
    s_drawData.ibSize += sizeof(uint16_t) * 6;
}

void Renderer2D::end() {
    float L = 0;
    float R = 100;
    float T = 0;
    float B = 100;
    s_drawData.projMat = glm::ortho(L, R, B, T);
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