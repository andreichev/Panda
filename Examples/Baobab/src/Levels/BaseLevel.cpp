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

void BaseLevel::start(Panda::World *world) {
    // Panda::Miren::setClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    baseShader =
        Miren::createShader("shaders/base/base_vertex.glsl", "shaders/base/base_fragment.glsl");
    LOG_INFO("WORLD GENERATION STARTED");
    chunksStorage = Foundation::makeShared<ChunksStorage>();
    LOG_INFO("WORLD GENERATED");
    Foundation::Shared<Panda::Entity> cameraEntity = world->instantiateEntity();
    Foundation::Shared<Panda::Camera> camera = Foundation::makeShared<Panda::Camera>();
    cameraEntity->addComponent(camera);
    camera->setFieldOfView(60.f);
    camera->setShader(baseShader);
    Foundation::Shared<CameraMove> cameraMove = Foundation::makeShared<CameraMove>();
    cameraMove->setCamera(camera);
    cameraEntity->addComponent(cameraMove);
    cameraEntity->getTransform()->translate(ChunksStorage::WORLD_SIZE_X / 2,
        ChunksStorage::WORLD_SIZE_Y / 4,
        ChunksStorage::WORLD_SIZE_Z / 2);
    cameraEntity->getTransform()->rotate(25.f, 0.f, 0.f);

    Foundation::Shared<BlocksCreation> blocksCreation = Foundation::makeShared<BlocksCreation>();
    blocksCreation->setChunksStorage(chunksStorage);
    blocksCreation->setCamera(camera);
    cameraEntity->addComponent(blocksCreation);

    Foundation::Shared<FullScreenToggle> fullScreenToggle =
        Foundation::makeShared<FullScreenToggle>();
    cameraEntity->addComponent(fullScreenToggle);

    world->getUIView()->addSubview(Foundation::makeShared<UICrosshair>());

    Foundation::Shared<Panda::Entity> chunkEntity = world->instantiateEntity();
    texture = Miren::createTextureFromFile("textures/Texture.png");
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Panda::MeshData meshData = VoxelMeshGenerator::makeOneChunkMesh(
                    *chunksStorage.get(), indexX, indexY, indexZ, true);
                Foundation::Shared<Panda::Mesh> mesh =
                    Foundation::makeShared<Panda::Mesh>(meshData, true, texture, baseShader);
                chunksStorage
                    ->chunks[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                             indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMesh(mesh.get());
                chunkEntity->addComponent(mesh);
            }
        }
    }

    LOG_INFO("BASE LEVEL STARTED!");
}
