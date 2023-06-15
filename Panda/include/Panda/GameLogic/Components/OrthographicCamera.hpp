#pragma once

#include "Panda/GameLogic/Component.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Panda/GameLogic/Components/Transform.hpp"

namespace Panda {

struct OrthographicCameraBounds {
    float left, right;
    float bottom, top;

    float getWidth() {
        return right - left;
    }
    float getHeight() {
        return top - bottom;
    }
};

class OrthographicCamera : public Component, TransformDelegate {
public:
    OrthographicCamera();
    ~OrthographicCamera() override;
    void initialize() override;
    void update(double deltaTime) override {}
    const glm::mat4 &getProjectionMatrix() const {
        return m_projectionMatrix;
    }
    const glm::mat4 &getViewMatrix() const {
        return m_viewMatrix;
    }
    const glm::mat4 &getViewProjectionMatrix() const {
        return m_viewProjectionMatrix;
    }
    const OrthographicCameraBounds &getBounds() const {
        return m_bounds;
    }
    float getZoomLevel() const {
        return m_zoom;
    }
    void setZoomLevel(float level);

    void updateViewportSize(Size size);
    Point screenCoordToWorld(Point coord);

    // MARK: - Delegate
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;

private:
    void updateProjectionMatrix();
    void updateViewMatrix();

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_viewProjectionMatrix;
    OrthographicCameraBounds m_bounds;
    Size m_screenSize;
    float m_aspectRatio;
    float m_zoom;
    Foundation::Shared<Transform> m_transform;
};

} // namespace Panda
