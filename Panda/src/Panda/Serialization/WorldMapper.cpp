#include "Panda/Serialization/WorldMapper.hpp"
#include "Panda/GameLogic/Components/SkyComponent.hpp"

namespace Panda {

void WorldMapper::fillWorld(World &world, const WorldDto &worldDto) {
    world.clear();
    for (auto entityDto : worldDto.entities) {
        Entity entity = world.instantiateEntity(entityDto.idComponent.id);
        // TAG COMPONENT
        { entity.setName(entityDto.tagComponent.tag); }
        // TRANSFORM COMPONENT
        {
            TransformComponentDto transformDto = entityDto.transformComponent;
            TransformComponent &transform = entity.getTransform();
            transform.setPosition(transformDto.position);
            transform.setRotation(transformDto.rotation);
            transform.setScale(transformDto.scale);
        }
        // CAMERA COMPONENT
        if (entityDto.cameraComponent.has_value()) {
            CameraComponentDto &cameraComponentDto = entityDto.cameraComponent.value();
            auto &cameraComponent = entity.addComponent<CameraComponent>();
            cameraComponent.isPrimary = cameraComponentDto.isPrimary;
            WorldCameraDto worldCameraDto = cameraComponentDto.camera;
            cameraComponent.camera.setProjectionType(worldCameraDto.getProjectionType());
            cameraComponent.camera.setNear(worldCameraDto.zNear);
            cameraComponent.camera.setFar(worldCameraDto.zFar);
            cameraComponent.camera.setFieldOfView(worldCameraDto.fieldOfView);
            cameraComponent.camera.setOrthoSize(worldCameraDto.orthoSize);
        }
        // SPRITE RENDERER COMPONENT
        if (entityDto.spriteRendererComponent.has_value()) {
            entity.addComponent<SpriteRendererComponent>(entityDto.spriteRendererComponent.value());
        }
        // CUBE MAP COMPONENT
        if (entityDto.cubeMap.has_value()) {
            entity.addComponent<SkyComponent>();
        }
    }
    world.resetChanged();
}

WorldDto WorldMapper::toDto(const World &world) {
    WorldDto worldDto;
    World &_world = const_cast<World &>(world);
    for (auto entityId : _world.m_registry.storage<entt::entity>()) {
        if (!_world.m_registry.valid(entityId)) {
            continue;
        }
        Entity entity(&_world.m_registry, (id_t)(entityId), &_world);
        EntityDto entityDto;
        // ID COMPONENT
        { entityDto.idComponent = entity.getComponent<IdComponent>(); }
        // TAG COMPONENT
        { entityDto.tagComponent = entity.getComponent<TagComponent>(); }
        // TRANSFORM COMPONENT
        {
            TransformComponentDto &transformDto = entityDto.transformComponent;
            TransformComponent &transform = entity.getTransform();
            transformDto.position = transform.getPosition();
            transformDto.rotation = transform.getRotation();
            transformDto.scale = transform.getScale();
        }
        // CAMERA COMPONENT
        if (entity.hasComponent<CameraComponent>()) {
            CameraComponent &cameraComponent = entity.getComponent<CameraComponent>();
            CameraComponentDto cameraComponentDto;
            cameraComponentDto.isPrimary = cameraComponent.isPrimary;
            WorldCameraDto worldCameraDto;
            worldCameraDto.setProjectionType(cameraComponent.camera.getProjectionType());
            worldCameraDto.zNear = cameraComponent.camera.getNear();
            worldCameraDto.zFar = cameraComponent.camera.getFar();
            worldCameraDto.fieldOfView = cameraComponent.camera.getFieldOfView();
            worldCameraDto.orthoSize = cameraComponent.camera.getOrthoSize();
            cameraComponentDto.camera = worldCameraDto;
            entityDto.cameraComponent = cameraComponentDto;
        }
        // SPRITE RENDERER COMPONENT
        if (entity.hasComponent<SpriteRendererComponent>()) {
            entityDto.spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();
        }
        // CUBE MAP COMPONENT
        if (entity.hasComponent<SkyComponent>()) {
            entityDto.cubeMap = CubeMapDto();
        }
        worldDto.entities.push_back(entityDto);
    }
    return worldDto;
}

} // namespace Panda