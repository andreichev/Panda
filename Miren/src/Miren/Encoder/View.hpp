#pragma once

#include "Miren/Base.hpp"

namespace Miren {

struct View {
    View()
        : m_clearColor(0x000000ff)
        , m_viewport(Rect::zero())
        , m_frameBuffer(MIREN_INVALID_HANDLE)
        , m_clearAttachments() {}

    uint32_t m_clearColor;
    Rect m_viewport;
    FrameBufferHandle m_frameBuffer;
    std::vector<Clear> m_clearAttachments;
};

} // namespace Miren
