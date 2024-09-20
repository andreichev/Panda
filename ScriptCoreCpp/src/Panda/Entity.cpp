#include "Panda/Entity.hpp"

namespace Panda {

Entity::Entity()
    : m_id(-1) {}

Entity::Entity(Panda::id_t id)
    : m_id(id) {}

} // namespace Panda