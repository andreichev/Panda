#pragma once

#include "Bamboo/Component.hpp"

namespace Bamboo {

class TransformComponent final : public Component {
public:
    Vec3 getPosition();
    void setPosition(Vec3 pos);
};

} // namespace Bamboo