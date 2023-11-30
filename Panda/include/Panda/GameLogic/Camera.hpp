//
// Created by Michael Andreichev on 30.11.2023.
//

#pragma once

#include <glm/glm.hpp>

namespace Panda {

class Camera {
public:
    virtual glm::mat4 &getViewProjectionMatrix() = 0;
};

} // namespace Panda
