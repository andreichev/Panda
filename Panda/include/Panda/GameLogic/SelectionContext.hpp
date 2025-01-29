#pragma once

#include "Entity.hpp"

namespace Panda {

class SelectionContext final {
public:
    SelectionContext() {
        unselectAll();
    }

    std::vector<Entity> getSelectedEntities() {
        return m_selectedEntities;
    }

    bool isSelected(Entity entity) {
        return std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) !=
               m_selectedEntities.end();
    }

    void addSelectedEntity(Entity entity) {
        m_selectedEntities.push_back(entity);
        updateValues();
    }

    void addSelectedEntities(std::vector<Entity> entities) {
        m_selectedEntities.insert(m_selectedEntities.end(), entities.begin(), entities.end());
        updateValues();
    }

    void removeSelectedEntity(Entity entity) {
        m_selectedEntities.erase(
            std::remove(m_selectedEntities.begin(), m_selectedEntities.end(), entity),
            m_selectedEntities.end()
        );
        updateValues();
    }

    void unselectAll() {
        m_selectedEntities.clear();
        resetValues();
    }

    int selectedCount() {
        return m_selectedEntities.size();
    }

    glm::mat4 getMedianMatrix() {
        return m_medianMatrix;
    }

    glm::vec3 getMedianPosition() {
        return m_medianPosition;
    }

private:
    void resetValues() {
        m_medianPosition = glm::vec3(0.0f);
        m_medianScale = glm::vec3(1.0f);
        m_medianRotation = glm::vec3(0.0f);
        m_medianMatrix = glm::mat4(1.f);
    }

    void updateValues() {
        resetValues();
        for (auto entity : m_selectedEntities) {
            auto &transformComponent = entity.getTransform();
            m_medianPosition += transformComponent.getPosition();
            m_medianScale += transformComponent.getScale();
            m_medianRotation += transformComponent.getRotationEuler();
        }
        m_medianPosition /= m_selectedEntities.size();
        m_medianScale /= m_selectedEntities.size();
        m_medianRotation /= m_selectedEntities.size();
        m_medianMatrix = glm::translate(glm::mat4(1.0f), m_medianPosition) *
                         glm::toMat4(glm::quat(m_medianRotation)) *
                         glm::scale(glm::mat4(1.0f), m_medianScale);
    }

    std::vector<Entity> m_selectedEntities;
    glm::vec3 m_medianPosition;
    glm::vec3 m_medianScale;
    glm::vec3 m_medianRotation;
    glm::mat4 m_medianMatrix;
};

} // namespace Panda