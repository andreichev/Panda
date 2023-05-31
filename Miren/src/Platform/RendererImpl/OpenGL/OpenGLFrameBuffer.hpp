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
    void bind();
    void unbind();

private:
    void checkStatus();

    FrameBufferSpecification spec;
    uint32_t m_id;
};

} // namespace Miren
