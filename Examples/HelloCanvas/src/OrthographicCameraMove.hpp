//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>
#include <Panda/GameLogic/Components/Camera.hpp>

class OrthographicCameraMove : public Panda::NativeScript {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;
    inline void setCamera(Panda::Camera *camera) {
        m_camera = camera;
    }

private:
    Panda::Camera *m_camera;
    Panda::Transform *m_transform;
    float m_cameraTranslationSpeed = 1.0f;
    float m_cameraRotationSpeed = 180.0f;
    double m_zoom;
    bool m_rotation;
};
