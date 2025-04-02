//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>
#include <Panda/GameLogic/Components/ParticleSystem.hpp>

class ParticlesEmitter final {
public:
    ParticlesEmitter(
        Panda::TransformComponent *transform,
        Panda::WorldCamera *camera,
        Panda::Renderer2D *renderer2D
    );
    void update(double deltaTime);
    void onImGuiRender();

private:
    void emit(float screenX, float screenY);

    Panda::Renderer2D *m_renderer2D;
    Panda::ParticleSystem m_particleSystem;
    Panda::WorldCamera *m_camera;
    Panda::TransformComponent *m_cameraTransform;
    Foundation::Shared<Panda::Texture> m_texture;
    float degree = 0.f;
    float colorFactor = 0.f;
};
