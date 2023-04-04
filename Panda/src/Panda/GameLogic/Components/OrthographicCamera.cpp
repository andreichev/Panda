#include "Panda/GameLogic/Components/OrthographicCamera.hpp"

#include "Panda/Application/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Panda {

OrthographicCamera::OrthographicCamera()
    : m_projectionMatrix(1.0f)
    , m_viewMatrix(1.0f)
    , m_viewProjectionMatrix(1.0f)
    , m_bounds()
    , m_screenSize(Application::get()->getWindow()->getSize())
    , m_aspectRatio(1.0f)
    , m_zoom(1.0f)
    , m_transform(nullptr) {
    LOG_INFO("Orthographic camera created, viewport size: {}, {}",
        m_screenSize.width,
        m_screenSize.height);
}

OrthographicCamera::~OrthographicCamera() {
    m_transform->removeDelegate(this);
    Application::get()->removeWindowSizeListener(this);
}

void OrthographicCamera::initialize() {
    m_transform = getEntity().getTransform();
    m_transform->addDelegate(this);
    Application::get()->addWindowSizeListener(this);
    windowSizeChanged(m_screenSize);
}

void OrthographicCamera::updateProjectionMatrix() {
    m_projectionMatrix =
        glm::ortho(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top, -1.0f, 1.0f);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void OrthographicCamera::updateViewMatrix() {
    glm::vec3 position = m_transform->getPosition();
    glm::vec3 rotation = m_transform->getRotation();
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    m_viewMatrix = glm::inverse(transform);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void OrthographicCamera::setZoomLevel(float level) {
    m_zoom = level;
    m_bounds = {-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom};
    updateProjectionMatrix();
}

Point OrthographicCamera::screenCoordToWorld(Point coord) {
    float x = coord.x;
    float y = coord.y;
    auto bounds = getBounds();
    auto pos = m_transform->getPosition();
    x = (x / m_screenSize.width) * bounds.getWidth() - bounds.getWidth() * 0.5f;
    y = bounds.getHeight() * 0.5f - (y / m_screenSize.height) * bounds.getHeight();
    return Point(x + pos.x, y + pos.y);
}

// MARK: - Window size delegate

void OrthographicCamera::windowSizeChanged(Size size) {
    m_screenSize = size;
    m_aspectRatio = size.width / size.height;
    m_bounds = {-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom};
    updateProjectionMatrix();
}

// MARK: - Transform Delegate

void OrthographicCamera::transformChanged(glm::vec4 position, glm::vec3 rotation) {
    updateViewMatrix();
}

} // namespace Panda