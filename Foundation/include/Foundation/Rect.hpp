//
// Created by Admin on 18.06.2022.
//

#pragma once

#include "Point.hpp"
#include "Size.hpp"

namespace Panda {

struct FRect {
    FPoint origin;
    FSize size;

    FRect()
        : origin()
        , size() {}

    FRect(float x, float y, float width, float height)
        : origin(x, y)
        , size(width, height) {}

    inline static FRect zero() {
        return FRect();
    }

    inline bool isZero() {
        return origin.isZero() && size.isZero();
    }
};

struct UIRect {
    UIPoint origin;
    UISize size;

    UIRect()
        : origin()
        , size() {}

    UIRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        : origin(x, y)
        , size(width, height) {}

    inline static UIRect zero() {
        return UIRect();
    }

    inline bool isZero() {
        return origin.isZero() && size.isZero();
    }
};

} // namespace Panda