//
// Created by Admin on 19/03/2022.
//

#pragma once

#include "OrthographicCameraMove.hpp"
#include "ParticlesEmitter.hpp"
#include "GameAssetHandler.hpp"

#include <Panda.hpp>

class ExampleLayer : public Panda::Layer {
public:
    ExampleLayer(Fern::Window *window);
    void onAttach() override;
    void onDetach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Fern::Event *event) override;

private:
    Fern::Window *m_window;
    Panda::WorldCamera m_camera;
    Panda::Renderer2D m_renderer2d;
    Panda::TransformComponent m_cameraTransform;
    GameAssetHandler m_assetHandler;
    ParticlesEmitter m_emitter;
    OrthographicCameraMove m_cameraMove;
};
