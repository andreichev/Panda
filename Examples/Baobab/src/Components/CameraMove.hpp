//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>

class CameraMove : public Panda::NativeScript {
public:
    void initialize() override;
    void update(double deltaTime) override;
    inline void setCamera(Panda::CameraComponent *camera) {
        m_camera = camera;
    }

private:
    float m_mouseSpeed = 0.2f;
    float m_moveSpeed = 20.0f;
    Panda::Transform *m_transform;
    Panda::CameraComponent *m_camera;
    Panda::Window *m_window;
};
