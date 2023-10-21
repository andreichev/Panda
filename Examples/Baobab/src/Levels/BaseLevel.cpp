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

#include <Panda.hpp>
#include <Miren/Miren.hpp>

void BaseLevel::start(Panda::World *world) {
    Miren::setViewClear(0, 0x3D75C9FF);
    Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
        "shaders/ground/ground_vertex.glsl", "shaders/ground/ground_fragment.glsl");
    m_groundShader = Miren::createProgram(programAsset.getMirenProgramCreate());
    LOG_INFO("WORLD GENERATION STARTED");
    m_chunksStorage = Foundation::makeShared<ChunksStorage>();
    LOG_INFO("WORLD GENERATED");
    Panda::Entity chunkEntity = world->instantiateEntity();
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
                Panda::MeshData meshData = VoxelMeshGenerator::makeOneChunkMesh(
                    layoutHandle, *m_chunksStorage.get(), indexX, indexY, indexZ, true);
                Panda::DynamicMeshComponent &meshComponent =
                    chunkEntity.getComponent<Panda::DynamicMeshComponent>();
                Foundation::Shared<Panda::DynamicMesh> dynamicMesh =
                    Foundation::makeShared<Panda::DynamicMesh>();
                meshComponent.meshes.push_back(dynamicMesh);
                dynamicMesh->create(meshData, m_blocksTileTexture, m_groundShader);
                m_chunksStorage
                    ->chunks[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                             indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMesh(dynamicMesh.get());
            }
        }
    }

    Panda::Entity cameraEntity = world->instantiateEntity();
    Panda::Camera &camera = cameraEntity.addNativeScript<Panda::Camera>();
    camera.setFieldOfView(60.f);
    Panda::Application::get()->getRenderer3D().setCamera(&camera);
    CameraMove &cameraMove = cameraEntity.addNativeScript<CameraMove>();
    cameraMove.setCamera(&camera);
    cameraEntity.getTransform().translate(ChunksStorage::WORLD_SIZE_X / 2,
        ChunksStorage::WORLD_SIZE_Y / 4,
        ChunksStorage::WORLD_SIZE_Z / 2);
    cameraEntity.getTransform().rotate(25.f, 0.f, 0.f);

    BlocksCreation &blocksCreation = cameraEntity.addNativeScript<BlocksCreation>();
    blocksCreation.setChunksStorage(m_chunksStorage);
    blocksCreation.setCamera(&camera);
    blocksCreation.setLayoutHandle(layoutHandle);

    cameraEntity.addNativeScript<FullScreenToggle>();

    LOG_INFO("BASE LEVEL STARTED!");
}

BaseLevel::~BaseLevel() {
    Miren::deleteProgram(m_groundShader);
    Miren::deleteTexture(m_blocksTileTexture);
}
