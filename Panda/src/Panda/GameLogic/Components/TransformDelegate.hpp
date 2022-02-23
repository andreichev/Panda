//
// Created by Admin on 10.02.2022.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Panda {

class TransformDelegate {
public:
    virtual void transformChanged(glm::vec4 position, glm::vec3 rotation) = 0;
};

} // namespace Panda
