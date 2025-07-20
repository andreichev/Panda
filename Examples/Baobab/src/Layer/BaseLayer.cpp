//
// Created by Admin on 07.02.2022.
//

#include "BaseLayer.hpp"
#include "Model/VoxelMeshGenerator.hpp"
#include "Model/Vertex.hpp"
#include "Components/CameraMove.hpp"
#include "Components/BlocksCreation.hpp"
#include "Components/FullScreenToggle.hpp"
#include "Components/UI/RootView.hpp"
#include "Panda/GameLogic/Components/SkyComponent.hpp"

#include <Panda.hpp>
#include <Fern/Events/KeyEvents.hpp>
#include <Fern/Events/WindowEvents.hpp>
#include <PandaUI/PandaUI.hpp>
#include <Miren/Miren.hpp>

BaseLayer::BaseLayer(Fern::Window *window)
    : m_window(window)
    , m_blocksCreation(&m_transform)
    , m_cameraMove(&m_transform)
    , m_groundShader() {}

void BaseLayer::onAttach() {
    Miren::setViewClear(0, 0x3D75C9FF);
    m_groundShader = Foundation::makeShared<Panda::ShaderAsset>(
        "shaders/ground/ground_vertex.glsl", "shaders/ground/ground_fragment.glsl"
    );
    Miren::TextureCreate textureCreate =
        Panda::AssetImporterBase::load2DTexture("textures/BlocksTile.png");
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = Miren::NEAREST;
    m_blocksTileTexture = Foundation::makeShared<Panda::TextureAsset>(textureCreate);
    Miren::VertexLayoutHandle layoutHandle =
        Miren::createVertexLayout(Vertex::createBufferLayout());
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Panda::MeshData meshData = VoxelMeshGenerator::makeOneChunkMesh(
                    layoutHandle, m_chunksStorage, indexX, indexY, indexZ, true
                );
                Panda::MeshAsset &dynamicMesh =
                    m_chunksStorage
                        .chunks
                            [indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                             indexX * ChunksStorage::SIZE_X + indexZ]
                        .getMesh();
                std::vector<Panda::TextureBinding> bindings = {{"texture1", m_blocksTileTexture}};
                Foundation::Shared<Panda::MaterialAsset> material =
                    Foundation::makeShared<Panda::MaterialAsset>(m_groundShader, bindings);
                dynamicMesh.create(meshData, material);
            }
        }
    }

    m_camera.setFieldOfView(60.f);
    auto windowSize = m_window->getSize();
    m_camera.setViewportSize({windowSize.width, windowSize.height});
    Miren::Rect viewport = Miren::Rect(
        0,
        0,
        windowSize.width * m_window->getDpi().width,
        windowSize.height * m_window->getDpi().height
    );
    Miren::setViewport(0, viewport);
    m_transform.translate(
        {ChunksStorage::WORLD_SIZE_X / 2,
         ChunksStorage::WORLD_SIZE_Y / 4,
         ChunksStorage::WORLD_SIZE_Z / 2}
    );
    m_transform.rotateEuler({glm::degrees(25.f), 0.f, 0.f});

    m_blocksCreation.setChunksStorage(&m_chunksStorage);
    m_blocksCreation.setCameraMove(&m_cameraMove);
    m_blocksCreation.setLayoutHandle(layoutHandle);

    PandaUI::initialize();
    Foundation::Shared<RootView> view = PandaUI::makeView<RootView>();
    PandaUI::Context::shared().setRootView(view);
    PandaUI::Context::shared().updateViewportSize({windowSize.width, windowSize.height});

    LOG_INFO("GAME INITIALIZED!");
}

void BaseLayer::onDetach() {}

void BaseLayer::onUpdate(double deltaTime) {
    m_renderer2d.begin(Panda::Renderer2D::Mode::DEFAULT, 0);
    m_renderer3d.begin(0);

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
    m_blocksCreation.update(deltaTime);
    m_cameraMove.update(deltaTime);

    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                auto &mesh = m_chunksStorage
                                 .chunks
                                     [indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                                      indexX * ChunksStorage::SIZE_X + indexZ]
                                 .getMesh();
                glm::mat4 transform(1.f);
                m_renderer3d.submit(transform, &mesh);
            }
        }
    }

    m_renderer2d.end();
    m_renderer3d.end();
}

void BaseLayer::onImGuiRender() {
    m_blocksCreation.onImGuiRender();
}

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
