//
// Created by Michael Andreichev on 03.05.2024.
//

#include "WorldSerializer.hpp"

namespace Panda {

WorldSerializer::WorldSerializer(World *world)
    : m_world(world) {}

void WorldSerializer::serialize(const std::filesystem::path &filepath) {}

bool WorldSerializer::deserialize(const std::filesystem::path &filepath) {
    return false;
}

} // namespace Panda