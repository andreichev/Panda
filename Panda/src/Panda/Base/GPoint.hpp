//
// Created by Admin on 18.06.2022.
//

#pragma once

namespace Panda {

struct GPoint {
    float x;
    float y;

    GPoint()
        : x(0)
        , y(0) {}

    GPoint(float x, float y)
        : x(x)
        , y(y) {}
};

} // namespace Panda