#pragma once

#include "Miren/Base.hpp"

namespace Miren {

struct View {
    View()
        : m_clearColor(0x000000ff)
        , m_viewport(Rect::zero()) {}

    uint32_t m_clearColor;
    Rect m_viewport;
};

} // namespace Miren
