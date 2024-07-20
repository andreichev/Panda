#pragma once

#include "Panda/Serialization/WorldDto.hpp"

namespace Panda {

class WorldMapper {
public:
    void fillWorld(World &world, const WorldDto &dto);
    WorldDto toDto(const World &world);
};

} // namespace Panda