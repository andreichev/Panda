//
// Created by Michael Andreichev on 30.11.2023.
//

#pragma once

#include <glm/glm.hpp>

#include "Panda/Base/Base.hpp"

namespace Panda {

class Camera {
public:
    virtual glm::mat4 &getProjection() = 0;
    virtual void setViewportSize(Size size) = 0;
    virtual Size getViewportSize() = 0;
};

} // namespace Panda
