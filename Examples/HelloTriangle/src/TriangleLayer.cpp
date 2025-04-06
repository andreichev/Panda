//
// Created by Admin on 19/03/2022.
//

#include "TriangleLayer.hpp"

#include <Fern/Events/KeyEvents.hpp>
#include <Fern/Events/WindowEvents.hpp>

TriangleLayer::~TriangleLayer() {}

TriangleLayer::TriangleLayer()
    : m_vertexBuffer()
    , m_indexBuffer()
    , m_shader() {}

void TriangleLayer::onAttach() {
    using namespace Miren;

    Panda::ProgramData programAsset = Panda::AssetLoaderEditor::loadProgram(
        "default-shaders/checker/checker_vertex.glsl",
        "default-shaders/checker/checker_fragment.glsl"
    );
    m_shader = createProgram(programAsset.getMirenProgramCreate());

    float rightEdge = 0.5f;
    float topEdge = 0.5f;
    float leftEdge = -0.5f;
    float bottomEdge = -0.5f;
    float *data = new float[8]{
        rightEdge, topEdge, leftEdge, topEdge, leftEdge, bottomEdge, rightEdge, bottomEdge
    };
    uint32_t *indices = new uint32_t[6]{0, 1, 2, 0, 2, 3};

    VertexBufferLayoutData layoutData;
    layoutData.pushFloat(2);
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer = createVertexBuffer(data, sizeof(float) * 8, vertexLayout);
    m_indexBuffer = createIndexBuffer(indices, BufferElementType::UnsignedInt, 6);

    Fern::Window *window = Panda::Application::get()->getMainWindow();
    auto size = window->getSize();
    Miren::Rect viewport = Miren::Rect(
        0, 0, size.width * window->getDpi().width, size.height * window->getDpi().height
    );
    Miren::setViewport(0, viewport);
}

void TriangleLayer::onDetach() {}

void TriangleLayer::onEvent(Fern::Event *event) {
    if (event->type == Fern::EventType::QuitRequest ||
        event->type == Fern::EventType::WindowCloseRequest) {
        event->isHandled = true;
        Panda::Application::get()->close();
    } else if (event->type == Fern::EventType::KeyReleased) {
        const Fern::KeyReleasedEvent *ev = static_cast<const Fern::KeyReleasedEvent *>(event);
        if (ev->key == Fern::Key::ESCAPE) { Panda::Application::get()->close(); }
    } else if (event->type == Fern::EventType::WindowResize) {
        Fern::Window *window = Panda::Application::get()->getMainWindow();
        window->getDrawingContext()->update();
        const Fern::WindowResizeEvent *ev = static_cast<const Fern::WindowResizeEvent *>(event);
        Panda::Size size = {
            static_cast<float>(ev->getWidth()), static_cast<float>(ev->getHeight())
        };
        Miren::Rect viewport = Miren::Rect(
            0, 0, size.width * window->getDpi().width, size.height * window->getDpi().height
        );
        Miren::setViewport(0, viewport);
    }
}

void TriangleLayer::onImGuiRender() {}

void TriangleLayer::onUpdate(double deltaTime) {
    Miren::setShader(m_shader);
    Miren::setVertexBuffer(m_vertexBuffer);
    Miren::setIndexBuffer(m_indexBuffer, 0, 6);
    Miren::submit(0);
}
