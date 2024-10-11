#include "Panda/Entity.hpp"

namespace Panda {

Entity::Entity()
    : m_id(0) {}

Entity::Entity(UUID id)
    : m_id(id) {}

} // namespace Panda