//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>
#include <Panda/GameLogic/Components/OrthographicCamera.hpp>

class OrthographicCameraMove : public Panda::Component {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;
    inline void setCamera(Foundation::Shared<Panda::OrthographicCamera> camera) {
        m_camera = camera;
    }

private:
    Foundation::Shared<Panda::OrthographicCamera> m_camera;
    Foundation::Shared<Panda::Transform> m_transform;
    float m_cameraTranslationSpeed = 1.0f;
    float m_cameraRotationSpeed = 180.0f;
    double m_zoom;
    bool m_rotation;
};