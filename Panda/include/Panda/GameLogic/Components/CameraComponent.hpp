//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/GameLogic/Camera.hpp"
#include "Panda/GameLogic/NativeScript.hpp"
#include "Panda/Events/WindowSizeObserver.hpp"
#include "Transform.hpp"
#include "TransformObserver.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

class CameraComponent final : public Camera, public NativeScript, TransformObserver {
public:
    CameraComponent();
    ~CameraComponent() override;
    void initialize() override;
    void setFieldOfView(float degrees);
    void update(double deltaTime) override {}
    glm::mat4 &getViewProjectionMatrix() override {
        m_viewProjection = m_projection * m_view;
        return m_viewProjection;
    }
    /// Первый вариант: дистанция от камеры задается в мировых координатах
    Vec3 screenCoordToWorld(Vec2 screen, float distance);
    /// Второй вариант: дистанция от камеры задается буфером глубины.
    /// Значение z от -1 до 1 для OpenGL или от 0 до 1 для Vulkan.
    Vec3 screenCoordToWorld(Vec3 screen);
    inline glm::vec4 getFront() const {
        return m_front;
    }
    inline glm::vec4 getUp() const {
        return m_up;
    }
    inline glm::vec4 getRight() const {
        return m_right;
    }

    void viewportSizeChanged(Size size);

    inline Size getViewportSize() {
        return m_viewportSize;
    }

    inline float getFieldOfView() {
        return m_fieldOfView;
    }

    // MARK: - Observer
    void transformChanged(glm::vec4 position, glm::vec3 rotation) override;

private:
    void updateVectors();
    void updateProjectionMatrix();
    void updateViewMatrix();

    Size m_viewportSize;
    Transform *m_transform;
    glm::mat4 m_rotationMatrix;
    glm::vec4 m_front;
    glm::vec4 m_up;
    glm::vec4 m_right;
    glm::vec3 m_target;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_viewProjection;
    float m_fieldOfView;
};

} // namespace Panda
