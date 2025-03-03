//
// Created by Michael Andreichev on 15.01.2024.
//

#pragma once

#include "Panda/Base/Base.hpp"

#include <glm/glm.hpp>

namespace Panda {

namespace Math {

    glm::vec3 smoothDamp(
        glm::vec3 current,
        glm::vec3 target,
        glm::vec3 &currentVelocity,
        float smoothTime,
        float deltaTime,
        float maxSpeed = FLT_MAX
    );

} // namespace Math

} // namespace Panda