#pragma once

#include "Miren/GraphicsContext.hpp"

namespace Miren {

#ifndef __OBJC__
typedef void *id;
#endif

class GLOsxContext final : public GraphicsContext {
public:
    void create() override;
    void flip() override;
    void setCurrent() override;
    void update() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    id context;
};

} // namespace Miren
