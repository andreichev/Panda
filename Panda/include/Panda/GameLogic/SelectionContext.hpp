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

    std::vector<Entity> getManipulatingEntities() {
        return m_manipulatingEntities;
    }

    bool isSelected(Entity entity) {
        return std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) !=
               m_selectedEntities.end();
    }

    void addSelectedEntity(Entity entity, bool needToCalculateMedian = true) {
        if (isSelected(entity)) { return; }
        m_selectedEntities.push_back(entity);
        //  Далее избегаем того, что при перемещении родителя трансформация будет применяться и к
        //  детям.
        std::vector<Entity> manipulating;
        for (auto item : m_manipulatingEntities) {
            if (entity.isDescendantOf(item)) {
                // Если выбранный элемент находится в иерархии уже выбранных - ничего не делаем.
                return;
            } else if (entity.isAncestorOf(item)) {
                // Если выбранный элемент является родителем - теперь он будет манипулировать.
                continue;
            }
            manipulating.push_back(item);
        }
        manipulating.push_back(entity);
        m_manipulatingEntities = manipulating;
        if (needToCalculateMedian) { updateValues(); }
    }

    void addSelectedEntities(std::vector<Entity> entities) {
        for (auto entity : entities) {
            addSelectedEntity(entity, false);
        }
        updateValues();
    }

    void removeSelectedEntity(Entity entity) {
        m_selectedEntities.erase(
            std::remove(m_selectedEntities.begin(), m_selectedEntities.end(), entity),
            m_selectedEntities.end()
        );
        m_manipulatingEntities.erase(
            std::remove(m_manipulatingEntities.begin(), m_manipulatingEntities.end(), entity),
            m_manipulatingEntities.end()
        );
        updateValues();
    }

    void unselectAll() {
        m_selectedEntities.clear();
        m_manipulatingEntities.clear();
        resetValues();
    }

    int selectedCount() {
        return m_selectedEntities.size();
    }

    bool empty() {
        return m_selectedEntities.empty();
    }

    glm::mat4 getMedianMatrix() {
        return m_medianMatrix;
    }

    glm::vec3 getMedianPosition() {
        return m_medianPosition;
    }

    void resetValues() {
        m_medianPosition = glm::vec3(0.0f);
        m_medianScale = glm::vec3(1.0f);
        m_medianRotation = glm::vec3(0.0f);
        m_medianMatrix = glm::mat4(1.f);
    }

    void updateValues() {
        resetValues();
        for (auto entity : m_selectedEntities) {
            auto transformComponent = entity.calculateWorldSpaceTransform();
            // auto transformComponent = entity.getTransform();
            m_medianPosition += transformComponent.getPosition();
            m_medianScale += transformComponent.getScale();
            m_medianRotation += transformComponent.getRotationEuler();
        }
        m_medianPosition /= m_selectedEntities.size();
        m_medianScale /= m_selectedEntities.size();
        m_medianRotation /= m_selectedEntities.size();
        m_medianMatrix = glm::translate(glm::mat4(1.0f), m_medianPosition) *
                         glm::toMat4(glm::quat(glm::radians(m_medianRotation))) *
                         glm::scale(glm::mat4(1.0f), m_medianScale);
    }

private:
    std::vector<Entity> m_selectedEntities;
    std::vector<Entity> m_manipulatingEntities;
    glm::vec3 m_medianPosition;
    glm::vec3 m_medianScale;
    glm::vec3 m_medianRotation;
    glm::mat4 m_medianMatrix;
};

} // namespace Panda