#include "Panda/Serialization/WorldMapper.hpp"
#include "Panda/GameLogic/Components/SkyComponent.hpp"

namespace Panda {

void WorldMapper::fillWorld(World &world, const WorldDto &worldDto) {
    world.clear();
    for (auto entityDto : worldDto.entities) {
        Entity entity = world.instantiateEntity(entityDto.id);
        // TAG COMPONENT
        { entity.setName(entityDto.tagComponent.tag); }
        // RELATIONSHIP COMPONENT
        { entity.setComponent<RelationshipComponent>(entityDto.relationshipComponent); }
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
            cameraComponent.camera.setProjectionType(worldCameraDto.projectionType);
            cameraComponent.camera.setNear(worldCameraDto.zNear);
            cameraComponent.camera.setFar(worldCameraDto.zFar);
            cameraComponent.camera.setFieldOfView(worldCameraDto.fieldOfView);
            cameraComponent.camera.setOrthoSize(worldCameraDto.orthoSize);
        }
        // SPRITE RENDERER COMPONENT
        if (entityDto.spriteRendererComponent.has_value()) {
            SpriteRendererComponentDto &spriteRendererDto =
                entityDto.spriteRendererComponent.value();
            auto &spriteRenderer = entity.addComponent<SpriteRendererComponent>();
            spriteRenderer.color = spriteRendererDto.color;
            spriteRenderer.cols = spriteRendererDto.cols;
            spriteRenderer.rows = spriteRendererDto.rows;
            spriteRenderer.index = spriteRendererDto.index;
            spriteRenderer.materialId = spriteRendererDto.material;
        }
        // CUBE MAP COMPONENT
        if (entityDto.cubeMapComponent.has_value()) { entity.addComponent<SkyComponent>(); }
        // PHYSICS 2D
        if (entityDto.rigidbody2dComponent.has_value()) {
            Rigidbody2DComponentDto &rigidbody2dDto = entityDto.rigidbody2dComponent.value();
            auto &rigidbody2d = entity.addComponent<Rigidbody2DComponent>();
            rigidbody2d.type = rigidbody2dDto.type;
            rigidbody2d.fixedRotation = rigidbody2dDto.fixedRotation;
        }
        if (entityDto.boxCollider2dComponent.has_value()) {
            BoxCollider2DComponentDto &boxCollider2dDto = entityDto.boxCollider2dComponent.value();
            auto &boxCollider2d = entity.addComponent<BoxCollider2DComponent>();
            boxCollider2d.isSensor = boxCollider2dDto.isSensor;
            boxCollider2d.offset = boxCollider2dDto.offset;
            boxCollider2d.size = boxCollider2dDto.size;
            boxCollider2d.density = boxCollider2dDto.density;
            boxCollider2d.friction = boxCollider2dDto.friction;
            boxCollider2d.restitution = boxCollider2dDto.restitution;
        }
        // SCRIPT LIST COMPONENT
        {
            ScriptListComponentDto scriptsComponentDto = entityDto.scriptListComponent;
            for (auto &scriptDto : scriptsComponentDto.scripts) {
                std::vector<ScriptField> fields;
                for (ScriptFieldDto &fieldDto : scriptDto.scriptFields) {
                    fields.emplace_back(
                        0, fieldDto.fieldId, fieldDto.name, fieldDto.type, fieldDto.value
                    );
                }
                entity.addScript(Panda::ExternalScript(0, scriptDto.name, fields));
            }
        }
    }
#ifdef PND_EDITOR
    world.setChanged(false);
    world.sort();
#endif
}

WorldDto WorldMapper::toDto(const World &world) {
    WorldDto worldDto;
    World &_world = const_cast<World &>(world);
    auto view = _world.m_registry.view<entt::entity>();
    for (auto entityId : view) {
        if (!_world.isValidEntt(entityId)) { continue; }
        Entity entity(entityId, &_world);
        EntityDto entityDto;
        // ID COMPONENT
        { entityDto.id = entity.getId(); }
        // TAG COMPONENT
        { entityDto.tagComponent = entity.getComponent<TagComponent>(); }
        // RELATIONSHIP COMPONENT
        { entityDto.relationshipComponent = entity.getComponent<RelationshipComponent>(); }
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
            worldCameraDto.projectionType = cameraComponent.camera.getProjectionType();
            worldCameraDto.zNear = cameraComponent.camera.getNear();
            worldCameraDto.zFar = cameraComponent.camera.getFar();
            worldCameraDto.fieldOfView = cameraComponent.camera.getFieldOfView();
            worldCameraDto.orthoSize = cameraComponent.camera.getOrthoSize();
            cameraComponentDto.camera = worldCameraDto;
            entityDto.cameraComponent = cameraComponentDto;
        }
        // SPRITE RENDERER COMPONENT
        if (entity.hasComponent<SpriteRendererComponent>()) {
            SpriteRendererComponent &spriteRenderer =
                entity.getComponent<SpriteRendererComponent>();
            SpriteRendererComponentDto spriteRendererDto;
            spriteRendererDto.color = spriteRenderer.color;
            spriteRendererDto.cols = spriteRenderer.cols;
            spriteRendererDto.rows = spriteRenderer.rows;
            spriteRendererDto.index = spriteRenderer.index;
            spriteRendererDto.material = spriteRenderer.materialId;
            entityDto.spriteRendererComponent = spriteRendererDto;
        }
        // PHYSICS
        if (entity.hasComponent<Rigidbody2DComponent>()) {
            Rigidbody2DComponent &rigidbody2d = entity.getComponent<Rigidbody2DComponent>();
            Rigidbody2DComponentDto rigidbody2dDto;
            rigidbody2dDto.type = rigidbody2d.type;
            rigidbody2dDto.fixedRotation = rigidbody2d.fixedRotation;
            entityDto.rigidbody2dComponent = rigidbody2dDto;
        }
        // BOX COLLIDER 2D
        if (entity.hasComponent<BoxCollider2DComponent>()) {
            BoxCollider2DComponent &boxCollider2d = entity.getComponent<BoxCollider2DComponent>();
            BoxCollider2DComponentDto boxCollider2dDto;
            boxCollider2dDto.isSensor = boxCollider2d.isSensor;
            boxCollider2dDto.offset = boxCollider2d.offset;
            boxCollider2dDto.size = boxCollider2d.size;
            boxCollider2dDto.density = boxCollider2d.density;
            boxCollider2dDto.friction = boxCollider2d.friction;
            boxCollider2dDto.restitution = boxCollider2d.restitution;
            entityDto.boxCollider2dComponent = boxCollider2dDto;
        }
        // CUBE MAP COMPONENT
        if (entity.hasComponent<SkyComponent>()) { entityDto.cubeMapComponent = CubeMapDto(); }
        // SCRIPT LIST COMPONENT
        {
            ScriptListComponentDto &scriptsComponentDto = entityDto.scriptListComponent;
            ScriptListComponent &scriptsComponent = entity.getComponent<ScriptListComponent>();
            for (auto &script : scriptsComponent.scripts) {
                std::vector<ScriptFieldDto> fields;
                for (ScriptField &field : script.getFields()) {
                    ScriptFieldDto fieldDto;
                    fieldDto.name = field.name;
                    fieldDto.fieldId = field.fieldId;
                    fieldDto.type = field.type;
                    fieldDto.value = field.value;
                    fields.emplace_back(fieldDto);
                }
                scriptsComponentDto.scripts.emplace_back(script.getName(), fields);
            }
        }
        worldDto.entities.push_back(entityDto);
    }
    return worldDto;
}

} // namespace Panda
