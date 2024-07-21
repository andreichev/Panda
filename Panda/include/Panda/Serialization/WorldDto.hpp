#pragma once

#include "Panda/GameLogic/Components/BaseComponents.hpp"
#include "Panda/Serialization/ComponentDtos.hpp"

namespace Panda {

struct EntityDto : public Rain::Codable {
    IdComponent idComponent;
    TagComponent tagComponent;
    TransformComponentDto transformComponent;
    std::optional<CameraComponentDto> cameraComponent;
    std::optional<SpriteRendererComponent> spriteRendererComponent;

    RAIN_FIELDS_BEGIN(EntityDto)
    RAIN_FIELD(idComponent)
    RAIN_FIELD(tagComponent)
    RAIN_FIELD(transformComponent)
    RAIN_FIELD(cameraComponent)
    RAIN_FIELD(spriteRendererComponent)
    RAIN_FIELDS_END
};

struct WorldDto : public Rain::Codable {
    std::vector<EntityDto> entities;

    RAIN_FIELDS_BEGIN(WorldDto)
    RAIN_FIELD(entities)
    RAIN_FIELDS_END
};

} // namespace Panda