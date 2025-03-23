#pragma once

namespace Panda {

template<typename TPrimitive, typename TComponent, typename GetOtherFunc>
bool isInconsistentPrimitive(const std::vector<Entity> &entities, GetOtherFunc func) {
    if (entities.size() < 2) { return false; }
    Entity firstEntity = entities[0];
    const TPrimitive first = func(firstEntity.getComponent<TComponent>());
    for (size_t i = 1; i < entities.size(); i++) {
        Entity entity = entities[i];
        if (!entity.hasComponent<TComponent>()) { continue; }
        const auto otherValue = func(entity.getComponent<TComponent>());
        if constexpr (std::is_same_v<TPrimitive, float>) {
            if (glm::epsilonNotEqual(otherValue, first, FLT_EPSILON)) { return true; }
        } else {
            if (otherValue != first) { return true; }
        }
    }
    return false;
}

using TransformGetVec3Func = std::function<glm::vec3(const TransformComponent &)>;

uint32_t
getInconsistentAxes(const std::vector<Entity> &entities, TransformGetVec3Func getValueFunc) {
    uint32_t axes = 0;
    if (isInconsistentPrimitive<float, TransformComponent>(
            entities, [&](const TransformComponent &transform) { return getValueFunc(transform).x; }
        )) {
        axes |= (uint32_t)VectorAxes::X;
    }
    if (isInconsistentPrimitive<float, TransformComponent>(
            entities, [&](const TransformComponent &transform) { return getValueFunc(transform).y; }
        )) {
        axes |= (uint32_t)VectorAxes::Y;
    }
    if (isInconsistentPrimitive<float, TransformComponent>(
            entities, [&](const TransformComponent &transform) { return getValueFunc(transform).z; }
        )) {
        axes |= (uint32_t)VectorAxes::Z;
    }
    return axes;
}

uint32_t getInconsistentAxes(const glm::vec3 &l, const glm::vec3 &r) {
    uint32_t axes = 0;
    if (glm::epsilonNotEqual(l.x, r.x, FLT_EPSILON)) { axes |= (uint32_t)VectorAxes::X; }
    if (glm::epsilonNotEqual(l.y, r.y, FLT_EPSILON)) { axes |= (uint32_t)VectorAxes::Y; }
    if (glm::epsilonNotEqual(l.z, r.z, FLT_EPSILON)) { axes |= (uint32_t)VectorAxes::Z; }
    return axes;
}

} // namespace Panda