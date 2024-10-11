#pragma once

#include "Base.hpp"

namespace Panda {

class Entity {
public:
    Entity();
    Entity(UUID id);

    inline bool issValid() {
        return m_id != 0;
    }

private:
    UUID m_id;

    friend class World;
};

} // namespace Panda