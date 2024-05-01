#include "EditorCamera.hpp"

#include <glm/ext.hpp>

namespace Panda {

EditorCamera::EditorCamera()
    : Camera({200, 200})
    , m_view(1.f)
    , m_fieldOfView(70.f) {}

void EditorCamera::updateProjectionMatrix() {
    float aspectRatio = m_viewportSize.width / m_viewportSize.height;
    m_projection =
        glm::perspective<float>(glm::radians(m_fieldOfView), aspectRatio, m_zNear, m_zFar);
}

} // namespace Panda
