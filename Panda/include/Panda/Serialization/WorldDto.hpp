#pragma once

#include "Panda/GameLogic/World.hpp"

#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Panda {

struct EntityDto : public Rain::Codable {
    IdComponent idComponent;
    TagComponent tagComponent;

    RAIN_FIELDS_BEGIN(EntityDto)
    RAIN_FIELD(idComponent)
    RAIN_FIELD(tagComponent)
    RAIN_FIELDS_END
};

struct WorldDto : public Rain::Codable {
    std::vector<EntityDto> entities;

    RAIN_FIELDS_BEGIN(WorldDto)
    RAIN_FIELD(entities)
    RAIN_FIELDS_END
};

} // namespace Panda