#include "Bamboo/Components/SpriteRendererComponent.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

Color SpriteRendererComponent::getColor() {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Color result;
    Panda::ExternalCalls::spriteRendererComponent_GetColor(
        entityHandle, &result.r, &result.g, &result.b, &result.a
    );
    return result;
}

void SpriteRendererComponent::setColor(Color color) {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Panda::ExternalCalls::spriteRendererComponent_SetColor(
        entityHandle, color.r, color.g, color.b, color.a
    );
}

void SpriteRendererComponent::setCell(int cols, int rows, int index) {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Panda::ExternalCalls::spriteRendererComponent_SetCell(entityHandle, cols, rows, index);
}

void SpriteRendererComponent::setTexture(Texture texture) {
    Panda::EntityHandle entityHandle = getEntity().getId();
    Panda::ExternalCalls::spriteRendererComponent_SetTexture(entityHandle, texture.getId());
}

} // namespace Bamboo
