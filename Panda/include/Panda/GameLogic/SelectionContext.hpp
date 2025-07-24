#pragma once

#include "Entity.hpp"

#include <unordered_set>

namespace Panda {

class SelectionContext final {
public:
    SelectionContext() = delete;

    static std::unordered_set<Entity> getSelectedEntities() {
        return m_selectedEntities;
    }

    static std::unordered_set<Entity> getManipulatingEntities() {
        return m_manipulatingEntities;
    }

    static bool isSelected(Entity entity) {
        return m_selectedEntities.contains(entity);
    }

    static void addSelectedEntity(Entity entity, bool needToCalculateMedian = true) {
        if (isSelected(entity)) { return; }
        m_selectedEntities.insert(entity);
        for (auto childId : entity.getChildEntities()) {}
        //  Далее избегаем того, что при перемещении родителя трансформация будет применяться и к
        //  детям.
        std::unordered_set<Entity> manipulating;
        for (auto item : m_manipulatingEntities) {
            if (entity.isDescendantOf(item)) {
                // Если выбранный элемент находится в иерархии уже выбранных - ничего не делаем.
                return;
            } else if (entity.isAncestorOf(item)) {
                // Если выбранный элемент является родителем - теперь он будет манипулировать.
                continue;
            }
            manipulating.insert(item);
        }
        manipulating.insert(entity);
        m_manipulatingEntities = manipulating;
        if (needToCalculateMedian) { updateValues(); }
    }

    static void addSelectedEntities(const std::unordered_set<Entity> &entities) {
        for (auto entity : entities) {
            addSelectedEntity(entity, false);
        }
        updateValues();
    }

    static void removeSelectedEntity(Entity entity, bool needToCalculateMedian = true) {
        m_selectedEntities.erase(entity);
        m_manipulatingEntities.erase(entity);
        if (needToCalculateMedian) { updateValues(); }
    }

    static void removeSelectedEntities(const std::vector<Entity> &entities) {
        for (auto entity : entities) {
            removeSelectedEntity(entity, false);
        }
        updateValues();
    }

    static void unselectAll() {
        m_selectedEntities.clear();
        m_manipulatingEntities.clear();
        resetValues();
    }

    static int selectedCount() {
        return m_selectedEntities.size();
    }

    static bool isEmpty() {
        return m_selectedEntities.empty();
    }

    static glm::mat4 getMedianMatrix() {
        return m_medianMatrix;
    }

    static glm::vec3 getMedianPosition() {
        return m_medianPosition;
    }

    static void resetValues() {
        m_medianPosition = glm::vec3(0.0f);
        m_medianScale = glm::vec3(1.0f);
        m_medianRotation = glm::quat();
        m_medianMatrix = glm::mat4(1.f);
    }

    static void updateValues() {
        resetValues();
        if (m_selectedEntities.empty()) { return; }
        auto it = m_selectedEntities.begin();
        Entity entity = *it;
        auto transformComponent = entity.calculateWorldSpaceTransform();
        m_medianPosition = transformComponent.getPosition();
        m_medianScale = transformComponent.getScale();
        m_medianRotation = transformComponent.getRotation();
        ++it;
        while (it != m_selectedEntities.end()) {
            entity = *it;
            transformComponent = entity.calculateWorldSpaceTransform();
            // auto transformComponent = entity.getTransform();
            m_medianPosition += transformComponent.getPosition();
            m_medianScale += transformComponent.getScale();
            m_medianRotation = glm::slerp(m_medianRotation, transformComponent.getRotation(), 0.5f);
            // if (glm::dot(m_medianRotation, transformComponent.getRotation()) < 0.0f) {
            //     m_medianRotation -= transformComponent.getRotation();
            // } else {
            //     m_medianRotation += transformComponent.getRotation();
            // }
            m_medianRotation = glm::normalize(m_medianRotation);
            ++it;
        }
        m_medianPosition /= m_selectedEntities.size();
        m_medianScale /= m_selectedEntities.size();
        m_medianMatrix = glm::translate(glm::mat4(1.0f), m_medianPosition) *
                         glm::toMat4(m_medianRotation) * glm::scale(glm::mat4(1.0f), m_medianScale);
    }

private:
    static std::unordered_set<UUID> m_selectedAssets;
    static std::unordered_set<Entity> m_selectedEntities;
    static std::unordered_set<Entity> m_manipulatingEntities;
    static glm::vec3 m_medianPosition;
    static glm::vec3 m_medianScale;
    static glm::quat m_medianRotation;
    static glm::mat4 m_medianMatrix;
};

} // namespace Panda