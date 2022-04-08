//
// Created by Admin on 07.02.2022.
//

#include "sndbxpch.hpp"
#include "BaseLevel.hpp"
#include "Model/VoxelMeshGenerator.hpp"
#include "Components/CameraMove.hpp"
#include "Panda.hpp"

void BaseLevel::start(Panda::World *world) {
    // Panda::Miren::setClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    baseShader = Panda::Miren::createShader("shaders/base/base_vertex.glsl", "shaders/base/base_fragment.glsl");
    PND_INFO("WORLD GENERATION STARTED");
    chunksStorage = Panda::createUnique<ChunksStorage>();
    PND_INFO("WORLD GENERATED");
    Panda::Shared<Panda::Entity> cameraEntity = world->instantiateEntity();
    Panda::Shared<Panda::Camera> camera = Panda::createShared<Panda::Camera>();
    cameraEntity->addComponent(camera);
    camera->setFieldOfView(60.f);
    camera->setShader(baseShader);
    cameraEntity->addComponent(Panda::createShared<CameraMove>());
    cameraEntity->getTransform()->translate(
        ChunksStorage::WORLD_SIZE_X / 2, ChunksStorage::WORLD_SIZE_Y / 3, ChunksStorage::WORLD_SIZE_Z / 2);
    cameraEntity->getTransform()->rotate((float)(M_PI / 4.f), (float)M_PI, 0.f);

    Panda::Shared<Panda::Entity> chunkEntity = world->instantiateEntity();
    texture = Panda::Miren::createTexture("textures/Texture.png");
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Panda::MeshData meshData = VoxelMeshGenerator::makeOneChunkMesh(*chunksStorage.get(), indexX, indexY, indexZ, true);
                if(meshData.indicesCount == 0) { continue; }
                Panda::Shared<Panda::Mesh> mesh = Panda::createShared<Panda::Mesh>(meshData, false, texture, baseShader);
                chunksStorage->chunks[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z + indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMesh(mesh.get());
                chunkEntity->addComponent(mesh);
            }
        }
    }

    PND_INFO("BASE LEVEL STARTED!");
}
