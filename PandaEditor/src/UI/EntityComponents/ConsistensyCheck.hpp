#pragma once

namespace Panda {

template<typename TPrimitive, typename TComponent, typename GetOtherFunc>
bool isInconsistentPrimitive(const std::unordered_set<Entity> &entities, GetOtherFunc func) {
    if (entities.size() < 2) { return false; }
    auto it = entities.begin();
    const TPrimitive first = func((*it).getComponent<TComponent>());
    while (it != entities.end()) {
        Entity entity = *it;
        if (!entity.hasComponent<TComponent>()) { continue; }
        const auto otherValue = func(entity.getComponent<TComponent>());
        if constexpr (std::is_same_v<TPrimitive, float>) {
            if (glm::epsilonNotEqual(otherValue, first, FLT_EPSILON)) { return true; }
        } else {
            if (otherValue != first) { return true; }
        }
        it++;
    }
    return false;
}

using TransformGetVec3Func = std::function<glm::vec3(const TransformComponent &)>;

uint32_t
getInconsistentAxes(const std::unordered_set<Entity> &entities, TransformGetVec3Func getValueFunc) {
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