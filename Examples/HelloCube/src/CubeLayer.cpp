//
// Created by Admin on 06/04/2022.
//

#include "CubeLayer.hpp"

#include <Fern/Events/KeyEvents.hpp>
#include <Fern/Events/WindowEvents.hpp>
#include <imgui.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    float light;

    Vertex(float x, float y, float z, float u, float v, float light)
        : pos(x, y, z)
        , uv(u, v)
        , light(light) {}
};

CubeLayer::~CubeLayer() {}

CubeLayer::CubeLayer(Fern::Window *window)
    : m_window(window) {}

void CubeLayer::onAttach() {
    using namespace Miren;
    auto vertices = new Vertex[24]{
        // Front
        Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f), // 0
        Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f),  // 1
        Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 2
        Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // 3
        // Back
        Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 4
        Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 5
        Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f),   // 6
        Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f),  // 7
        // Top
        Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 8
        Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),  // 11
        Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 10
        Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f),  // 9
        // Bottom
        Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 12
        Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 13
        Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 14
        Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // 15
        // Left
        Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 16
        Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f),  // 17
        Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 18
        Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f),  // 19
        // Right
        Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 20
        Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 23
        Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 22
        Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f)   // 21
    };

    // clang-format off
    auto indices = new uint32_t[36]{
        0, 1, 2, 2, 3, 0, // Front
        4, 5, 6, 6, 7, 4, // Back
        8, 9, 10, 10, 11, 8, // Top
        12, 13, 14, 14, 15, 12, // Bottom
        16, 17, 18, 18, 19, 16, // Left
        20, 21, 22, 22, 23, 20 // Right
    };
    // clang-format on

    VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushFloat(1);
    VertexLayoutHandle layoutHandle = createVertexLayout(layoutData);
    vertexBuffer = createVertexBuffer(vertices, 24 * sizeof(Vertex), layoutHandle);
    indexBuffer = createIndexBuffer(indices, BufferElementType::UnsignedInt, 36);

    Panda::TextureData textureAsset = Panda::AssetLoaderEditor::loadTexture("textures/arbuz1.png");
    texture = createTexture(textureAsset.getMirenTextureCreate());
    Panda::ProgramData programAsset = Panda::AssetLoaderEditor::loadProgram(
        "default-shaders/renderer3d/base_vertex.glsl",
        "default-shaders/renderer3d/base_fragment.glsl"
    );
    shader = createProgram(programAsset.getMirenProgramCreate());
    setShader(shader);

    Fern::Size windowSize = m_window->getSize();
    Miren::Rect viewport = Miren::Rect(
        0,
        0,
        windowSize.width * m_window->getDpi().width,
        windowSize.height * m_window->getDpi().height
    );
    Miren::setViewport(0, viewport);
    view = glm::lookAt(
        glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
    );
    // view = glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective<float>(40.f, windowSize.x / windowSize.y, 0.1f, 1000.0f);
    // projectionMatrix = glm::perspective(90.f, 1.0f, 0.1f, 1000.0f);
    model = glm::mat4(1.f);
    translate = glm::vec3(0.f, 0.f, 0.f);
    viewProjection = projection * view;

    setUniform(shader, "projViewMtx", &viewProjection[0][0], UniformType::Mat4);
    time = 0;
}

void CubeLayer::onDetach() {}

void CubeLayer::onUpdate(double deltaTime) {
    time += deltaTime;
    model = glm::mat4(1.f);
    model = glm::scale(glm::mat4(1.f), glm::vec3(abs(sin(time)) + 1.f, abs(sin(time)) + 1.f, 1.f));
    model = glm::translate(model, translate);
    model = glm::rotate(model, (float)time, glm::vec3(1.f, 1.f, 0.f));
    viewProjection = projection * view;

    Miren::setShader(shader);
    Miren::setTexture(texture, 0);
    Miren::setUniform(shader, "model", &model[0][0], Miren::UniformType::Mat4);
    Miren::setUniform(shader, "projViewMtx", &viewProjection[0][0], Miren::UniformType::Mat4);

    Miren::setVertexBuffer(vertexBuffer);
    Miren::setIndexBuffer(indexBuffer, 0, 36);
    Miren::submit(0);
}

void CubeLayer::onEvent(Fern::Event *event) {
    if (event->type == Fern::EventType::QuitRequest ||
        event->type == Fern::EventType::WindowCloseRequest) {
        event->isHandled = true;
        Panda::Application::get()->close();
    } else if (event->type == Fern::EventType::KeyReleased) {
        const Fern::KeyReleasedEvent *ev = static_cast<const Fern::KeyReleasedEvent *>(event);
        if (ev->key == Fern::Key::ESCAPE) { Panda::Application::get()->close(); }
    } else if (event->type == Fern::EventType::WindowResize) {
        m_window->getDrawingContext()->update();
        const Fern::WindowResizeEvent *ev = static_cast<const Fern::WindowResizeEvent *>(event);
        Panda::Size size = {
            static_cast<float>(ev->getWidth()), static_cast<float>(ev->getHeight())
        };
        Miren::Rect viewport = Miren::Rect(
            0, 0, size.width * m_window->getDpi().width, size.height * m_window->getDpi().height
        );
        Miren::setViewport(0, viewport);
    }
}

void CubeLayer::onImGuiRender() {
    ImGui::ShowDemoWindow();
}
