//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Panda {

class Shader {
public:
    virtual ~Shader() = default;
    virtual void use() = 0;
    virtual void setUniform(const char *name, glm::mat4 value) = 0;
};

} // namespace Panda
