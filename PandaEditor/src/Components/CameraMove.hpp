//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>
#include <Panda/GameLogic/Components/CameraComponent.hpp>

namespace Panda {

class CameraMove : public NativeScript {
public:
    void initialize() override;
    void update(double deltaTime) override;
    void onImGuiRender() override;
    inline void setCamera(CameraComponent *camera) {
        m_camera = camera;
    }

private:
    CameraComponent *m_camera;
    Transform *m_transform;
    float m_mouseSpeed = 0.2f;
    float m_moveSpeed = 20.0f;
};

} // namespace Panda
