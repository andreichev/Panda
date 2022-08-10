//
// Created by Admin on 18.06.2022.
//

#pragma once

namespace Panda {

struct FPoint {
    float x;
    float y;

    FPoint()
        : x(0)
        , y(0) {}

    FPoint(float x, float y)
        : x(x)
        , y(y) {}

    inline bool isZero() {
        return x == 0 && y == 0;
    }
};

struct UIPoint {
    uint32_t x;
    uint32_t y;

    UIPoint()
        : x(0)
        , y(0) {}

    UIPoint(uint32_t x, uint32_t y)
        : x(x)
        , y(y) {}

    inline bool isZero() {
        return x == 0 && y == 0;
    }
};

} // namespace Panda