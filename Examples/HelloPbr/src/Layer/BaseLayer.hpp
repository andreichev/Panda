//
// Created by Admin on 07.02.2022.
//

#pragma once

#include "Model/TerrainMeshGenerator.hpp"
#include "Components/CameraMove.hpp"
#include "Components/FullScreenToggle.hpp"

#include <Panda/GameLogic/Components/TransformComponent.hpp>
#include <Panda/GameLogic/Components/SkyComponent.hpp>
#include <Panda.hpp>

class BaseLayer : public Panda::Layer {
public:
    BaseLayer(Fern::Window *window);
    ~BaseLayer();

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
    Panda::SkyComponent m_skyComponent;
    TerrainMeshGenerator m_meshGenerator;
    Panda::MeshAsset m_mesh;

    Miren::ProgramHandle m_shader;
    Miren::TextureHandle m_heightMapTexture;
    Miren::TextureHandle m_colorTexture;
    Miren::TextureHandle m_patternTexture;
    Panda::Renderer2D m_renderer2d;
    Panda::Renderer3D m_renderer3d;
};
