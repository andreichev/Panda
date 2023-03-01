//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>

class CameraMove : public Panda::Component {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;
    inline void setCamera(Foundation::Shared<Panda::Camera> camera) {
        m_camera = camera;
    }

private:
    float m_mouseSpeed = 0.2f;
    float m_moveSpeed = 20.0f;
    Foundation::Shared<Panda::Transform> m_transform;
    Foundation::Shared<Panda::Camera> m_camera;
    Panda::Window *m_window;
};
