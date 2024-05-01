#pragma once

#include <Panda/GameLogic/Camera.hpp>

namespace Panda {

class EditorCamera : public Camera {
public:
    EditorCamera();
    ~EditorCamera() = default;

private:
    void updateProjectionMatrix() override;

    glm::mat4 m_view;
    float m_fieldOfView;
};

} // namespace Panda