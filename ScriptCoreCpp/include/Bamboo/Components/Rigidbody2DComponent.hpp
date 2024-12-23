#pragma once

#include "Bamboo/Component.hpp"

namespace Bamboo {

class Rigidbody2DComponent final : public Component {
public:
    void applyForce(Vec2 force);
    void applyLinearImpulse(Vec2 impulse);
    Vec2 getLinearVelocity();
    void setLinearVelocity(Vec2 velocity);
    float getMass();
    float getFriction();
    void setFriction(float friction);
};

} // namespace Bamboo