#pragma once

#include "Bamboo/Component.hpp"

namespace Bamboo {

class TransformComponent final : public Component {
public:
    Vec3 getPosition();
    void setPosition(Vec3 pos);
    Vec3 getRotationEuler();
    void setRotationEuler(Vec3 degrees);
    Vec3 getScale();
    void setScale(Vec3 scale);
};

} // namespace Bamboo