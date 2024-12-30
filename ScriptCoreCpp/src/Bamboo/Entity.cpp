#include "Bamboo/Entity.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

Entity::Entity()
    : m_id(0) {}

Entity::Entity(EntityHandle id)
    : m_id(id) {}

const char *Entity::getName() {
    return Panda::ExternalCalls::entity_GetName(m_id);
}

} // namespace Bamboo