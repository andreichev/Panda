//
// Created by Admin on 19/03/2022.
//

#include "TriangleLevel.hpp"

class TriangleRenderer : public Panda::Component {
public:
    TriangleRenderer(int mVertexBuffer, int mIndexBuffer, int mShader)
        : m_vertexBuffer(mVertexBuffer)
        , m_indexBuffer(mIndexBuffer)
        , m_shader(mShader) {}

    void initialize() override {}

    void update(double deltaTime) override {
        Panda::Miren::setShader(m_shader);
        Panda::Miren::setVertexBuffer(m_vertexBuffer);
        Panda::Miren::setIndexBuffer(m_indexBuffer, 6);
        Panda::Miren::submit();
    }

private:
    Panda::VertexBufferHandle m_vertexBuffer;
    Panda::IndexBufferHandle m_indexBuffer;
    Panda::ShaderHandle m_shader;
};

void TriangleLevel::start(Panda::World *world) {
    Panda::ShaderHandle baseShader =
        Panda::Miren::createShader("shaders/checker/checker_vertex.glsl", "shaders/checker/checker_fragment.glsl");
    float rightEdge = 0.5f;
    float topEdge = 0.5f;
    float leftEdge = -0.5f;
    float bottomEdge = -0.5f;
    float *data = new float[]{rightEdge, topEdge, leftEdge, topEdge, leftEdge, bottomEdge, rightEdge, bottomEdge};
    uint32_t *indices = new uint32_t[]{0, 1, 2, 0, 2, 3};

    Panda::VertexBufferLayoutData layoutData;
    layoutData.push<float>(2);
    Panda::VertexLayoutHandle vertexLayout = Panda::Miren::createVertexLayout(layoutData);
    Panda::VertexBufferHandle vertexBuffer = Panda::Miren::createVertexBuffer(data, 32, vertexLayout);
    Panda::IndexBufferHandle indexBuffer = Panda::Miren::createIndexBuffer(indices, 6);

    Panda::Shared<TriangleRenderer> triangleRenderer = Panda::makeShared<TriangleRenderer>(vertexBuffer, indexBuffer, baseShader);

    Panda::Shared<Panda::Entity> entity = world->instantiateEntity();
    entity->addComponent(triangleRenderer);
}
