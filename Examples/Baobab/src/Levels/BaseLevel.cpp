//
// Created by Admin on 07.02.2022.
//

#include "pch.hpp"
#include "BaseLevel.hpp"
#include "Model/VoxelMeshGenerator.hpp"
#include "Model/Vertex.hpp"
#include "Components/CameraMove.hpp"
#include "Components/BlocksCreation.hpp"
#include "Components/FullScreenToggle.hpp"
#include "Components/UI/RootView.hpp"

#include <Panda.hpp>
#include <PandaUI/PandaUI.hpp>
#include <Miren/Miren.hpp>

class CameraSizeObserver final : public Panda::NativeScript, Panda::WindowSizeObserver {
public:
    void initialize() override {
        Panda::Application::get()->addWindowSizeObserver(this);
    }

    ~CameraSizeObserver() {
        Panda::Application::get()->removeWindowSizeObserver(this);
    }

    void update(double deltaTime) override {}

    void windowSizeChanged(Panda::Size size) override {
        m_camera->setViewportSize(size);
        PandaUI::Context::shared().updateViewportSize({size.width, size.height});
    }

    void setCamera(Panda::Camera *camera) {
        m_camera = camera;
    }

private:
    Panda::Camera *m_camera;
};

void BaseLevel::start(Panda::World *world) {
    Miren::setViewClear(0, 0x3D75C9FF);
    Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
        "shaders/ground/ground_vertex.glsl", "shaders/ground/ground_fragment.glsl"
    );
    m_groundShader = Miren::createProgram(programAsset.getMirenProgramCreate());
    LOG_INFO("WORLD GENERATION STARTED");
    m_chunksStorage = Foundation::makeShared<ChunksStorage>();
    LOG_INFO("WORLD GENERATED");
    Panda::TextureAsset textureAsset = Panda::AssetLoader::loadTexture("textures/BlocksTile.png");
    Miren::TextureCreate textureCreate = textureAsset.getMirenTextureCreate();
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = Miren::NEAREST;
    m_blocksTileTexture = Miren::createTexture(textureCreate);
    Miren::VertexLayoutHandle layoutHandle =
        Miren::createVertexLayout(Vertex::createBufferLayout());
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Panda::Entity chunkEntity = world->instantiateEntity();
                Panda::MeshData meshData = VoxelMeshGenerator::makeOneChunkMesh(
                    layoutHandle, *m_chunksStorage.get(), indexX, indexY, indexZ, true
                );
                Panda::DynamicMeshComponent &meshComponent =
                    chunkEntity.getComponent<Panda::DynamicMeshComponent>();
                Panda::DynamicMesh &dynamicMesh = meshComponent.meshes.emplace_back();
                dynamicMesh.create(meshData, {{"texture1", m_blocksTileTexture}}, m_groundShader);
                m_chunksStorage
                    ->chunks
                        [indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                         indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMeshEntity(chunkEntity);
            }
        }
    }

    Panda::Entity cameraEntity = world->instantiateEntity();
    Panda::WorldCamera &camera = cameraEntity.addComponent<Panda::CameraComponent>().camera;
    camera.setFieldOfView(60.f);
    CameraMove &cameraMove = cameraEntity.addNativeScript<CameraMove>();
    CameraSizeObserver &cameraSizeObserver = cameraEntity.addNativeScript<CameraSizeObserver>();
    cameraSizeObserver.setCamera(&camera);
    cameraEntity.getTransform().translate(
        {ChunksStorage::WORLD_SIZE_X / 2,
         ChunksStorage::WORLD_SIZE_Y / 4,
         ChunksStorage::WORLD_SIZE_Z / 2}
    );
    cameraEntity.getTransform().rotateEuler({glm::degrees(25.f), 0.f, 0.f});

    BlocksCreation &blocksCreation = cameraEntity.addNativeScript<BlocksCreation>();
    blocksCreation.setChunksStorage(m_chunksStorage);
    blocksCreation.setCameraMove(&cameraMove);
    blocksCreation.setLayoutHandle(layoutHandle);

    cameraEntity.addNativeScript<FullScreenToggle>();

    PandaUI::initialize();
    Foundation::Shared<RootView> view = PandaUI::makeView<RootView>();
    PandaUI::Context::shared().setRootView(view);

    LOG_INFO("BASE LEVEL STARTED!");
}

BaseLevel::~BaseLevel() {
    Miren::deleteProgram(m_groundShader);
    Miren::deleteTexture(m_blocksTileTexture);
}
