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
    void reset();
    void attachTexture(
        FrameBufferAttachmentSpecification &spec, int samples, uint32_t width, uint32_t height);

    FrameBufferSpecification *specification;
    std::vector<uint32_t> m_colorAttachments;
    uint32_t m_depthAttachment;
    uint32_t m_id;
};

} // namespace Miren
