#pragma once

#include "Miren/Base.hpp"

#include <vector>

namespace Miren {

class RendererOpenGL;

class OpenGLFrameBuffer {
public:
    OpenGLFrameBuffer();
    void create(RendererOpenGL *renderer, FrameBufferSpecification specification);
    void terminate();
    void bind();
    void unbind();
    void readPixels(
        RendererOpenGL *renderer, int index, int x, int y, int width, int height, void *data
    );
    void clearIntAttachment(int index, int value);
    void clearUIntAttachment(int index, uint32_t value);

private:
    void checkStatus();

    FrameBufferSpecification spec;
    uint32_t m_id;
};

} // namespace Miren
