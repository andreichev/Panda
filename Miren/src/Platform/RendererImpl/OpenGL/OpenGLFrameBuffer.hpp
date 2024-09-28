#pragma once

#include "Miren/Base.hpp"

#include <vector>

namespace Miren {

class OpenGLFrameBuffer {
public:
    OpenGLFrameBuffer();
    void create(FrameBufferSpecification specification);
    void terminate();
    void bind();
    void unbind();
    void readPixels(int x, int y, int width, int height, void *data);

private:
    void checkStatus();

    FrameBufferSpecification spec;
    uint32_t m_id;
};

} // namespace Miren
