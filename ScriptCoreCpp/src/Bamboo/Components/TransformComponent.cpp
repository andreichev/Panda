#include "Bamboo/Components/TransformComponent.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

Vec3 TransformComponent::getPosition() {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Vec3 result;
    Panda::ExternalCalls::transformComponent_GetPosition(
        entityHandle, &result.x, &result.y, &result.z
    );
    return result;
}

void TransformComponent::setPosition(Vec3 pos) {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Panda::ExternalCalls::transformComponent_SetPosition(entityHandle, pos.x, pos.y, pos.z);
}

Vec3 TransformComponent::getRotationEuler() {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Vec3 result;
    Panda::ExternalCalls::transformComponent_GetRotationEuler(
        entityHandle, &result.x, &result.y, &result.z
    );
    return result;
}

void TransformComponent::setRotationEuler(Vec3 degrees) {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Panda::ExternalCalls::transformComponent_SetRotationEuler(
        entityHandle, degrees.x, degrees.y, degrees.z
    );
}

Vec3 TransformComponent::getScale() {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Vec3 result;
    Panda::ExternalCalls::transformComponent_GetScale(
        entityHandle, &result.x, &result.y, &result.z
    );
    return result;
}

void TransformComponent::setScale(Vec3 scale) {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Panda::ExternalCalls::transformComponent_SetScale(entityHandle, scale.x, scale.y, scale.z);
}

} // namespace Bamboo
