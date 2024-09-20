#include "Panda/World.hpp"

#include "OuterScriptHook.hpp"

namespace Panda {

std::optional<Entity> World::findByTag(const char *tag) {
    id_t entityId = ExternalCalls::world_FindByTag(tag);
    if (entityId == -1) {
        return {};
    }
    return Entity(entityId);
}

Entity World::createEntity(const char *tag) {
    id_t entityId = ExternalCalls::world_CreateEntity(tag);
    return Entity(entityId);
}

void World::destroyEntity(Panda::Entity entity) {
    ExternalCalls::world_DestroyEntity(entity.m_id);
}

} // namespace Panda