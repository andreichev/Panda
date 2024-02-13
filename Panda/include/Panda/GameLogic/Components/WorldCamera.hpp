#pragma once

#include "Panda/GameLogic/Camera.hpp"
#include "Panda/Base/Base.hpp"

namespace Panda {

// Universal camera (Orthographic or perspective)
class WorldCamera : public Camera {
public:
    enum class ProjectionType { PERSPECTIVE = 0, ORTHOGRAPHIC = 1 };

    WorldCamera();
    ~WorldCamera() = default;
    glm::mat4 &getProjection() override {
        return m_projection;
    };
    void setProjectionType(ProjectionType type);
    // MARK: Perspective parameters
    void setFieldOfView(float degrees);
    float getFieldOfView() {
        return m_fieldOfView;
    }
    // MARK: Orthographic parameters

    // MARK: Camera
    void setViewportSize(Size size) override;
    Size getViewportSize() override {
        return m_viewportSize;
    }
    ProjectionType getProjectionType() const {
        return m_projectionType;
    }

    // MARK: Util
    /// Первый вариант: дистанция от камеры задается в мировых координатах
    Vec3 screenCoordToWorld(glm::mat4 view, Vec2 screen, float distance);
    /// Второй вариант: дистанция от камеры задается буфером глубины.
    /// Значение z от -1 до 1 для OpenGL или от 0 до 1 для Vulkan.
    Vec3 screenCoordToWorld(glm::mat4 view, Vec3 screen);

private:
    void updateProjectionMatrix();

    ProjectionType m_projectionType;
    Size m_viewportSize;
    glm::mat4 m_projection;
    float m_fieldOfView;
};

} // namespace Panda