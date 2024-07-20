#pragma once

#include "Panda/Serialization/WorldDto.hpp"
#include "Panda/GameLogic/World.hpp"

namespace Panda {

class WorldMapper {
public:
    static void fillWorld(World &world, const WorldDto &dto);
    static WorldDto toDto(const World &world);
};

} // namespace Panda