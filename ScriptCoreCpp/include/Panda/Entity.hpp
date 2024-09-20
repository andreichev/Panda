#pragma once

#include "Base.hpp"

namespace Panda {

class Entity {
public:
    Entity();
    Entity(id_t id);

    inline bool issValid() {
        return m_id != -1;
    }

private:
    id_t m_id;

    friend class World;
};

} // namespace Panda