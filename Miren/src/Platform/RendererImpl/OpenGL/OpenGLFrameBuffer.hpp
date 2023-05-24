#pragma once

#include "Miren/Base.hpp"
#include <vector>

namespace Miren {

class OpenGLFrameBuffer {
public:
    OpenGLFrameBuffer();
    void create(FrameBufferSpecification specification);
    void terminate();
    void resize(uint32_t width, uint32_t height);

private:
    void checkStatus();

    FrameBufferSpecification specification;
    uint32_t m_id;
};

} // namespace Miren
