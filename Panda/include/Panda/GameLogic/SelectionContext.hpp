#pragma once

#include "Entity.hpp"
#include "World.hpp"

#include <unordered_set>

namespace Panda {

class SelectionContext final {
public:
    SelectionContext() = delete;

    static std::unordered_set<UUID> getSelectedEntities() {
        return s_selectedEntities;
    }

    static std::unordered_set<UUID> getManipulatingEntities() {
        return s_manipulatingEntities;
    }

    static bool isSelected(UUID id) {
        return s_selectedEntities.contains(id);
    }

    static void addSelectedEntity(UUID id, bool needToCalculateMedian = true) {
        if (isSelected(id)) { return; }
        s_selectedEntities.insert(id);
        World *world = GameContext::getCurrentWorld();
        if (!world) { return; }
        Entity entity = world->getById(id);
        for (auto childId : entity.getChildEntities()) {}
        //  Далее избегаем того, что при перемещении родителя трансформация будет применяться и к
        //  детям.
        std::unordered_set<UUID> manipulating;
        for (auto manipulatingId : s_manipulatingEntities) {
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
        s_manipulatingEntities = manipulating;
        if (needToCalculateMedian) { updateValues(); }
    }

    static void addSelectedEntities(const std::unordered_set<UUID> &ids) {
        for (auto id : ids) {
            addSelectedEntity(id, false);
        }
        updateValues();
    }

    static void removeSelectedEntity(UUID id, bool needToCalculateMedian = true) {
        s_selectedEntities.erase(id);
        s_manipulatingEntities.erase(id);
        if (needToCalculateMedian) { updateValues(); }
    }

    static void removeSelectedEntities(const std::unordered_set<UUID> &ids) {
        for (auto id : ids) {
            removeSelectedEntity(id, false);
        }
        updateValues();
    }

    static void unselectAll() {
        s_selectedEntities.clear();
        s_manipulatingEntities.clear();
        resetValues();
    }

    static int selectedCount() {
        return s_selectedEntities.size();
    }

    static bool isEmpty() {
        return s_selectedEntities.empty();
    }

    static glm::mat4 getMedianMatrix() {
        return s_medianMatrix;
    }

    static glm::vec3 getMedianPosition() {
        return s_medianPosition;
    }

    static void resetValues() {
        s_medianPosition = glm::vec3(0.0f);
        s_medianScale = glm::vec3(1.0f);
        s_medianRotation = glm::quat();
        s_medianMatrix = glm::mat4(1.f);
    }

    static void updateValues() {
        resetValues();
        if (s_selectedEntities.empty()) { return; }
        World *world = GameContext::getCurrentWorld();
        if (!world) { return; }
        auto it = s_selectedEntities.begin();
        UUID id = *it;
        Entity entity = world->getById(id);
        auto transformComponent = entity.calculateWorldSpaceTransform();
        s_medianPosition = transformComponent.getPosition();
        s_medianScale = transformComponent.getScale();
        s_medianRotation = transformComponent.getRotation();
        ++it;
        while (it != s_selectedEntities.end()) {
            id = *it;
            entity = world->getById(id);
            transformComponent = entity.calculateWorldSpaceTransform();
            // auto transformComponent = entity.getTransform();
            s_medianPosition += transformComponent.getPosition();
            s_medianScale += transformComponent.getScale();
            s_medianRotation = glm::slerp(s_medianRotation, transformComponent.getRotation(), 0.5f);
            // if (glm::dot(s_medianRotation, transformComponent.getRotation()) < 0.0f) {
            //     s_medianRotation -= transformComponent.getRotation();
            // } else {
            //     s_medianRotation += transformComponent.getRotation();
            // }
            s_medianRotation = glm::normalize(s_medianRotation);
            ++it;
        }
        s_medianPosition /= s_selectedEntities.size();
        s_medianScale /= s_selectedEntities.size();
        s_medianMatrix = glm::translate(glm::mat4(1.0f), s_medianPosition) *
                         glm::toMat4(s_medianRotation) * glm::scale(glm::mat4(1.0f), s_medianScale);
    }

private:
    static std::unordered_set<UUID> s_selectedAssets;
    static std::unordered_set<UUID> s_selectedEntities;
    static std::unordered_set<UUID> s_manipulatingEntities;
    static glm::vec3 s_medianPosition;
    static glm::vec3 s_medianScale;
    static glm::quat s_medianRotation;
    static glm::mat4 s_medianMatrix;
};

} // namespace Panda