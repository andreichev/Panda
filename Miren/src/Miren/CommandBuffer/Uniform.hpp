#pragma once

#include "Miren/Miren.hpp"

#include <Foundation/Logger.hpp>

namespace Miren {

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
        LOG_ERROR("Uniform type is undefined");
        return 0;
    }

    Uniform(ShaderHandle handle, const char *name, void *value, UniformDataType type)
        : handle(handle)
        , name(name)
        , value(value)
        , type(type) {
        size = getSizeOfType(type);
    }

    Uniform()
        : handle(0)
        , name(nullptr)
        , value(nullptr)
        , type(UniformDataType::Int)
        , size(0) {}
};

} // namespace Miren