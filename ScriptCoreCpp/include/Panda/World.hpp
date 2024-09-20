#pragma once

#include "Base.hpp"
#include "Entity.hpp"

#include <optional>

namespace Panda {

class World {
public:
    static std::optional<Entity> findByTag(const char *tag);
    static Entity createEntity(const char *tag);
    static void destroyEntity(Entity entity);
};

} // namespace Panda