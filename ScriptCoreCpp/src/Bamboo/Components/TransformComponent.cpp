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

} // namespace Bamboo
