#include "Panda/Serialization/WorldMapper.hpp"

namespace Panda {

void WorldMapper::fillWorld(World &world, const WorldDto &worldDto) {
    world.clear();
    for (auto entityDto : worldDto.entities) {
        Entity entity = world.instantiateEntity(entityDto.idComponent.id);
        entity.setName(entityDto.tagComponent.tag);
    }
}

WorldDto WorldMapper::toDto(const World &world) {
    WorldDto worldDto;
    World& _world = const_cast<World&>(world);
    for (auto entityId : _world.m_registry.storage<entt::entity>()) {
        Entity entity(&_world.m_registry, (id_t)(entityId), &_world);
        EntityDto entityDto;
        entityDto.idComponent = entity.getComponent<IdComponent>();
        entityDto.tagComponent = entity.getComponent<TagComponent>();
        worldDto.entities.push_back(entityDto);
    }
    return worldDto;
}

} // namespace Panda