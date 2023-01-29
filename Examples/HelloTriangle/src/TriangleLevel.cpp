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
        Miren::setShader(m_shader);
        Miren::setVertexBuffer(m_vertexBuffer);
        Miren::setIndexBuffer(m_indexBuffer, 0, 6);
        Miren::submit();
    }

private:
    Miren::VertexBufferHandle m_vertexBuffer;
    Miren::IndexBufferHandle m_indexBuffer;
    Miren::ShaderHandle m_shader;
};

void TriangleLevel::start(Panda::World *world) {
    using namespace Miren;

    ShaderHandle baseShader = createShader("shaders/checker/checker_vertex.glsl", "shaders/checker/checker_fragment.glsl");
    float rightEdge = 0.5f;
    float topEdge = 0.5f;
    float leftEdge = -0.5f;
    float bottomEdge = -0.5f;
    float *data = new float[8]{rightEdge, topEdge, leftEdge, topEdge, leftEdge, bottomEdge, rightEdge, bottomEdge};
    uint32_t *indices = new uint32_t[6]{0, 1, 2, 0, 2, 3};

    VertexBufferLayoutData layoutData;
    layoutData.pushFloat(2);
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    VertexBufferHandle vertexBuffer = createVertexBuffer(data, sizeof(float) * 8, vertexLayout);
    IndexBufferHandle indexBuffer = createIndexBuffer(indices, BufferElementType::UnsignedInt, 6);

    Foundation::Shared<TriangleRenderer> triangleRenderer = Foundation::makeShared<TriangleRenderer>(vertexBuffer, indexBuffer, baseShader);

    Foundation::Shared<Panda::Entity> entity = world->instantiateEntity();
    entity->addComponent(triangleRenderer);
}
