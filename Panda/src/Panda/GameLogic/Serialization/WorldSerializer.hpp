#pragma once

#include "Panda/GameLogic/World.hpp"

#include <filesystem>

namespace Panda {

class WorldSerializer final {
public:
    WorldSerializer(World *world);

    void serialize(const std::filesystem::path &filepath);
    bool deserialize(const std::filesystem::path &filepath);

private:
    World *m_world;
};

} // namespace Panda
