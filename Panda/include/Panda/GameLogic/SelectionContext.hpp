#pragma once

#include "Entity.hpp"
#include "World.hpp"

#include <unordered_set>

namespace Panda {

class SelectionContext final {
public:
    SelectionContext() = delete;

    static std::unordered_set<UUID> getSelectedEntities() {
        return m_selectedEntities;
    }

    static std::unordered_set<UUID> getManipulatingEntities() {
        return m_manipulatingEntities;
    }

    static bool isSelected(UUID id) {
        return m_selectedEntities.contains(id);
    }

    static void addSelectedEntity(UUID id, bool needToCalculateMedian = true) {
        if (isSelected(id)) { return; }
        m_selectedEntities.insert(id);
        World *world = GameContext::s_currentWorld;
        if (!world) { return; }
        Entity entity = world->getById(id);
        for (auto childId : entity.getChildEntities()) {}
        //  Далее избегаем того, что при перемещении родителя трансформация будет применяться и к
        //  детям.
        std::unordered_set<UUID> manipulating;
        for (auto manipulatingId : m_manipulatingEntities) {
            Entity manipulatingEntity = world->getById(manipulatingId);
            if (entity.isDescendantOf(manipulatingEntity)) {
                // Если выбранный элемент находится в иерархии уже выбранных - ничего не делаем.
                return;
            } else if (entity.isAncestorOf(manipulatingEntity)) {
                // Если выбранный элемент является родителем - теперь он будет манипулировать.
                continue;
            }
            manipulating.insert(manipulatingId);
        }
        manipulating.insert(id);
        m_manipulatingEntities = manipulating;
        if (needToCalculateMedian) { updateValues(); }
    }

    static void addSelectedEntities(const std::unordered_set<UUID> &ids) {
        for (auto id : ids) {
            addSelectedEntity(id, false);
        }
        updateValues();
    }

    static void removeSelectedEntity(UUID id, bool needToCalculateMedian = true) {
        m_selectedEntities.erase(id);
        m_manipulatingEntities.erase(id);
        if (needToCalculateMedian) { updateValues(); }
    }

    static void removeSelectedEntities(const std::unordered_set<UUID> &ids) {
        for (auto id : ids) {
            removeSelectedEntity(id, false);
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
        World *world = GameContext::s_currentWorld;
        if (!world) { return; }
        auto it = m_selectedEntities.begin();
        UUID id = *it;
        Entity entity = world->getById(id);
        auto transformComponent = entity.calculateWorldSpaceTransform();
        m_medianPosition = transformComponent.getPosition();
        m_medianScale = transformComponent.getScale();
        m_medianRotation = transformComponent.getRotation();
        ++it;
        while (it != m_selectedEntities.end()) {
            id = *it;
            entity = world->getById(id);
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
    static std::unordered_set<UUID> m_selectedEntities;
    static std::unordered_set<UUID> m_manipulatingEntities;
    static glm::vec3 m_medianPosition;
    static glm::vec3 m_medianScale;
    static glm::quat m_medianRotation;
    static glm::mat4 m_medianMatrix;
};

} // namespace Panda