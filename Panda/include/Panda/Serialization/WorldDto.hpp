#pragma once

#include "Panda/GameLogic/Components/BaseComponents.hpp"
#include "Panda/Serialization/ComponentDtos.hpp"

namespace Panda {

struct EntityDto : public Rain::Codable {
    UUID id;
    TagComponent tagComponent;
    RelationshipComponent relationshipComponent;
    TransformComponentDto transformComponent;
    std::optional<CameraComponentDto> cameraComponent;
    std::optional<SpriteRendererComponentDto> spriteRendererComponent;
    std::optional<CubeMapDto> cubeMapComponent;
    ScriptListComponentDto scriptListComponent;
    std::optional<Rigidbody2DComponentDto> rigidbody2dComponent;
    std::optional<BoxCollider2DComponentDto> boxCollider2dComponent;

    RAIN_FIELDS_BEGIN(EntityDto)
    RAIN_FIELD(id)
    RAIN_FIELD(tagComponent)
    RAIN_FIELD(relationshipComponent)
    RAIN_FIELD(transformComponent)
    RAIN_FIELD(cameraComponent)
    RAIN_FIELD(spriteRendererComponent)
    RAIN_FIELD(cubeMapComponent)
    RAIN_FIELD(scriptListComponent)
    RAIN_FIELD(rigidbody2dComponent)
    RAIN_FIELD(boxCollider2dComponent)
    RAIN_FIELDS_END
};

struct WorldDto : public Rain::Codable {
    std::vector<EntityDto> entities;

    RAIN_FIELDS_BEGIN(WorldDto)
    RAIN_FIELD(entities)
    RAIN_FIELDS_END
};

} // namespace Panda