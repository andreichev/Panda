//
// Created by Admin on 07.02.2022.
//

#include "BaseLayer.hpp"
#include "Model/Vertex.hpp"
#include "Components/UI/RootView.hpp"
#include "Model/PerlinNoise.hpp"

#include <Panda.hpp>
#include <Fern/Events/KeyEvents.hpp>
#include <Fern/Events/WindowEvents.hpp>
#include <PandaUI/PandaUI.hpp>
#include <Miren/Miren.hpp>

BaseLayer::~BaseLayer() {
    Miren::deleteProgram(m_shader);
    Miren::deleteTexture(m_colorTexture);
}

BaseLayer::BaseLayer(Fern::Window *window)
    : m_window(window)
    , m_cameraMove(&m_transform) {}

void BaseLayer::onAttach() {
    Miren::setViewClear(0, 0x3D75C9FF);
    Panda::ProgramData programAsset = Panda::AssetLoaderEditor::loadProgram(
        "shaders/ground/ground_vertex.glsl", "shaders/ground/ground_fragment.glsl"
    );
    m_shader = Miren::createProgram(programAsset.getMirenProgramCreate());
    m_cameraMove.setShader(m_shader);
    LOG_INFO("MESH GENERATION STARTED");

    Panda::TextureData colorTextureAsset =
        Panda::AssetLoaderEditor::loadTexture("textures/grass1.png");
    Miren::TextureCreate colorTextureCreate = colorTextureAsset.getMirenTextureCreate();
    colorTextureCreate.m_numMips = 4;
    colorTextureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
    m_colorTexture = Miren::createTexture(colorTextureCreate);

#ifdef HEIGHT_MAP
    Panda::TextureAsset heightTextureAsset = Panda::AssetLoader::loadTexture("textures/map2.png");
    int width = heightTextureAsset.m_width;
    int height = heightTextureAsset.m_height;
    float *heightMap = (float *)F_ALLOC(Foundation::getAllocator(), sizeof(float) * width * height);
    uint8_t *data = (uint8_t *)heightTextureAsset.m_data.data;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            int index = h * width + w;
            heightMap[index] = data[index * 4] / 255.f;
        }
    }
#else
    int width = 256;
    int height = 256;
    float *heightMap = (float *)F_ALLOC(Foundation::getAllocator(), sizeof(float) * width * height);
    PerlinNoise::generate2DGlm(123, 4, 1.0f, heightMap, width, height);
#endif

    Miren::VertexLayoutHandle layoutHandle =
        Miren::createVertexLayout(Vertex::createBufferLayout());
    Panda::MeshData meshData = m_meshGenerator.makeMesh(layoutHandle, width, height, heightMap);
    F_FREE(Foundation::getAllocator(), heightMap);

    m_mesh.create(
        meshData, {{"texture1", m_colorTexture}, {"iSky", m_skyComponent.getSkyTexture()}}, m_shader
    );

    PandaUI::initialize();
    Foundation::Shared<RootView> view = PandaUI::makeView<RootView>();
    PandaUI::Context::shared().setRootView(view);

    LOG_INFO("GAME INITIALIZED!");
}

void BaseLayer::onDetach() {}

void BaseLayer::onUpdate(double deltaTime) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    glm::mat4 viewProjMtx;
    glm::mat4 skyViewProjMtx;

    glm::mat4 viewMtx = glm::inverse(m_transform.getLocalTransform());
    glm::mat4 skyViewMtx = glm::inverse(m_transform.getSkyTransform());
    glm::mat4 projMtx = m_camera.getProjection();

    viewProjMtx = projMtx * viewMtx;
    skyViewProjMtx = projMtx * skyViewMtx;
    m_renderer2d.setViewProj(viewProjMtx);
    m_renderer3d.setViewProj(viewProjMtx);

    m_skyComponent.update(skyViewProjMtx);

    glm::mat4 transform(1.f);
    m_renderer3d.submit(transform, &m_mesh);

    m_cameraMove.update(deltaTime);

    m_renderer2d.end();
    m_renderer3d.end();
}

void BaseLayer::onImGuiRender() {}

void BaseLayer::onEvent(Fern::Event *event) {
    if (event->type == Fern::EventType::QuitRequest ||
        event->type == Fern::EventType::WindowCloseRequest) {
        event->isHandled = true;
        Panda::Application::get()->close();
    } else if (event->type == Fern::EventType::WindowResize) {
        const Fern::WindowResizeEvent *ev = static_cast<const Fern::WindowResizeEvent *>(event);
        Panda::Size size = {
            static_cast<float>(ev->getWidth()), static_cast<float>(ev->getHeight())
        };
        m_camera.setViewportSize(size);
        m_window->getDrawingContext()->update();
        Miren::Rect viewport = Miren::Rect(
            0, 0, size.width * m_window->getDpi().width, size.height * m_window->getDpi().height
        );
        Miren::setViewport(0, viewport);
    } else if (event->type == Fern::EventType::KeyReleased) {
        const Fern::KeyReleasedEvent *ev = static_cast<const Fern::KeyReleasedEvent *>(event);
        if (ev->key == Fern::Key::F) {
            m_window->setState(
                m_window->getState() == Fern::WindowState::WindowStateFullScreen
                    ? Fern::WindowState::WindowStateNormal
                    : Fern::WindowState::WindowStateFullScreen
            );
        } else if (ev->key == Fern::Key::ESCAPE) {
            Panda::Application::get()->close();
        }
    }
}
