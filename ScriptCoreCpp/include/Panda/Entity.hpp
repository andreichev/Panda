#pragma once

#include "Base.hpp"

namespace Panda {

class Entity {
private:
    Entity(id_t id);
    id_t m_id;
};

} // namespace Panda