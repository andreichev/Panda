#pragma once

#include "Miren/Miren.hpp"

#include <Foundation/Logger.hpp>

namespace Panda {

struct Uniform {
    ShaderHandle handle;
    const char *name;
    void *value;
    UniformDataType type;
    uint32_t size;

    static uint32_t getSizeOfType(UniformDataType type) {
        switch (type) {
            case UniformDataType::Int:
                return 4;
            case UniformDataType::Mat4:
                return sizeof(float) * 16;
        }
        PND_ERROR("Uniform type is undefined");
        return 0;
    }

    Uniform(ShaderHandle handle, const char *name, void *value, UniformDataType type)
        : handle(handle)
        , name(name)
        , value(value)
        , type(type) {
        size = getSizeOfType(type);
    }
};

} // namespace Panda