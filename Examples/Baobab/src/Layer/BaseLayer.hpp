//
// Created by Admin on 07.02.2022.
//

#pragma once

#include "Model/ChunksStorage.hpp"
#include "Components/BlocksCreation.hpp"
#include "Components/CameraMove.hpp"

#include <Panda/GameLogic/Components/TransformComponent.hpp>
#include <Panda.hpp>

class BaseLayer : public Panda::Layer {
public:
    BaseLayer(Fern::Window *window);

    void onAttach() override;
    void onDetach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Fern::Event *event) override;

private:
    Fern::Window *m_window;
    Panda::WorldCamera m_camera;
    CameraMove m_cameraMove;
    Panda::TransformComponent m_transform;
    ChunksStorage m_chunksStorage;
    BlocksCreation m_blocksCreation;
    Panda::AssetRef<Panda::ShaderAsset> m_groundShader;
    Panda::AssetRef<Panda::TextureAsset> m_blocksTileTexture;
    Panda::Renderer2D m_renderer2d;
    Panda::Renderer3D m_renderer3d;
};
