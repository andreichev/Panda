//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>
#include <Panda/GameLogic/Components/CameraComponent.hpp>

class OrthographicCameraMove : public Panda::NativeScript {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;
    inline void setCamera(Panda::CameraComponent *camera) {
        m_camera = camera;
    }

private:
    Panda::CameraComponent *m_camera;
    Panda::Transform *m_transform;
    float m_moveSpeed = 1.4f;
    float m_rotationSpeed = 180.0f;
};
