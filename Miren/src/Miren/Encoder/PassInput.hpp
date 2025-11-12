#pragma once

#include "Miren/Miren.hpp"

#include <Foundation/Logger.hpp>

namespace Miren {

struct PassInput {
    ProgramHandle handle;
    const char *name;
    RenderPassInputType type;
    void *data;
    uint16_t alignment;
    uint16_t size;

    PassInput(
        ProgramHandle handle, const char *name, RenderPassInputType type, void *data, uint16_t size
    )
        : handle(handle)
        , name(name)
        , type(type)
        , data(data)
        , alignment(0)
        , size(size) {}
};

} // namespace Miren