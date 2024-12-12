#include "Bamboo/Entity.hpp"

namespace Bamboo {

Entity::Entity()
    : m_id(0) {}

Entity::Entity(Panda::EntityHandle id)
    : m_id(id) {}

} // namespace Bamboo