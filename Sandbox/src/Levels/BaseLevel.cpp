//
// Created by Admin on 07.02.2022.
//

#include "sndbxpch.hpp"
#include "BaseLevel.hpp"
#include "Model/VoxelMeshGenerator.hpp"
#include "Components/CameraMove.hpp"

#include <Panda/GameLogic/Entity.hpp>
#include <Panda/Application/Application.hpp>
#include <Panda/Platform/RendererInit.hpp>
#include <Panda/GameLogic/Components/Camera.hpp>

void BaseLevel::start(Panda::World *world) {
    Panda::Application::get().getRenderer().setClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    baseShader = Panda::Shared<Panda::IShader>(
        Panda::RendererInit::createShader("shaders/base/base_vertex.glsl", "shaders/base/base_fragment.glsl", nullptr));
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
        ChunksStorage::WORLD_SIZE_X / 2, ChunksStorage::WORLD_SIZE_Y / 2, ChunksStorage::WORLD_SIZE_Z / 2);
    cameraEntity->getTransform()->rotate((float)(M_PI / 4.f), (float)M_PI, 0.f);

    texture = Panda::Shared<Panda::ITexture>(Panda::RendererInit::createTexture("textures/Texture.png"));
    for (int indexX = 0; indexX < ChunksStorage::SIZE_X; indexX++) {
        for (int indexY = 0; indexY < ChunksStorage::SIZE_Y; indexY++) {
            for (int indexZ = 0; indexZ < ChunksStorage::SIZE_Z; indexZ++) {
                Panda::Shared<Panda::Entity> chunkEntity = world->instantiateEntity();
                Panda::MeshData meshData = VoxelMeshGenerator::makeOneChunkMesh(*chunksStorage.get(), indexX, indexY, indexZ, true);
                Panda::Shared<Panda::Mesh> mesh = Panda::createShared<Panda::Mesh>(meshData, false, texture, baseShader);
                chunksStorage->chunks[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z + indexX * ChunksStorage::SIZE_X + indexZ]
                    .setMesh(mesh.get());
                chunkEntity->addComponent(mesh);
            }
        }
    }

    PND_INFO("BASE LEVEL STARTED!");
}