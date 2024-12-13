#include "Bamboo/World.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

std::optional<Entity> World::findByTag(const char *tag) {
    Panda::EntityHandle entityId = Panda::ExternalCalls::world_FindByTag(tag);
    if (entityId == 0) {
        return {};
    }
    return Entity(entityId);
}

Entity World::createEntity(const char *tag) {
    Panda::EntityHandle entityId = Panda::ExternalCalls::world_CreateEntity(tag);
    return Entity(entityId);
}

void World::destroyEntity(Entity entity) {
    Panda::ExternalCalls::world_DestroyEntity(entity.m_id);
}

} // namespace Bamboo