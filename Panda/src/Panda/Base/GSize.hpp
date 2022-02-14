//
// Created by Admin on 22.01.2022.
//

#pragma once

namespace Panda {

struct GSize {
    float width;
    float height;

    GSize()
        : width(0)
        , height(0) {}

    GSize(float width, float height)
        : width(width)
        , height(height) {}
};

} // namespace Panda