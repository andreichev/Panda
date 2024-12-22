#pragma once

#include "Base.hpp"

#include <cfloat>

namespace Bamboo {

namespace Math {

    Vec3 smoothDamp(
        Vec3 current,
        Vec3 target,
        Vec3 &currentVelocity,
        float smoothTime,
        float deltaTime,
        float maxSpeed = FLT_MAX
    );

}

} // namespace Bamboo