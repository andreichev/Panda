//
// Created by Admin on 07.02.2022.
//

#include "pch.hpp"
#include "BaseLevel.hpp"
#include "Model/VoxelMeshGenerator.hpp"
#include "Components/CameraMove.hpp"
#include "Components/BlocksCreation.hpp"
#include "Components/FullScreenToggle.hpp"
#include "Components/UI/UICrosshair.hpp"
#include "Panda.hpp"
#include "Miren/Miren.hpp"

void BaseLevel::start(Panda::World *world) {
    // Panda::Miren::setClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    Miren::setViewClear(0, 0x12212bff);
    Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
        "shaders/base/base_vertex.glsl", "shaders/base/base_fragment.glsl");
    baseShader = Miren::createProgram(programAsset.getMirenProgramCreate());
    LOG_INFO("WORLD GENERATION STARTED");
    chunksStorage = Foundation::makeShared<ChunksStorage>();
    LOG_INFO("WORLD GENERATED");
    Panda::Entity chunkEntity = world->instantiateEntity();
    Panda::TextureAsset textureAsset = Panda::AssetLoader::loadTexture("textures/Texture.png");
    texture = Miren::createTexture(textureAsset.getMirenTextureCreate());
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Panda::MeshData meshData = VoxelMeshGenerator::makeOneChunkMesh(
                    *chunksStorage.get(), indexX, indexY, indexZ, true);
                Panda::Mesh &mesh = chunkEntity.addNativeScript<Panda::Mesh>();
                mesh.create(meshData, true, texture, baseShader);
                chunksStorage
                    ->chunks[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                             indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMesh(&mesh);
            }
        }
    }

    Panda::Entity cameraEntity = world->instantiateEntity();
    Panda::Camera &camera = cameraEntity.addNativeScript<Panda::Camera>();
    camera.setFieldOfView(60.f);
    camera.setShader(baseShader);
    CameraMove &cameraMove = cameraEntity.addNativeScript<CameraMove>();
    cameraMove.setCamera(&camera);
    cameraEntity.getTransform().translate(ChunksStorage::WORLD_SIZE_X / 2,
        ChunksStorage::WORLD_SIZE_Y / 4,
        ChunksStorage::WORLD_SIZE_Z / 2);
    cameraEntity.getTransform().rotate(25.f, 0.f, 0.f);

    BlocksCreation &blocksCreation = cameraEntity.addNativeScript<BlocksCreation>();
    blocksCreation.setChunksStorage(chunksStorage);
    blocksCreation.setCamera(&camera);

    cameraEntity.addNativeScript<FullScreenToggle>();

    world->getUIView()->addSubview(Foundation::makeShared<UICrosshair>());

    LOG_INFO("BASE LEVEL STARTED!");
}
