//
// Created by Admin on 18.06.2022.
//

#pragma once

#include "GPoint.hpp"
#include "GSize.hpp"

namespace Panda {

struct GRect {
    GPoint origin;
    GSize size;

    GRect()
        : origin()
        , size() {}

    GRect(float x, float y, float width, float height)
        : origin(x, y)
        , size(width, height) {}

    inline static GRect zero() {
        return GRect();
    }
};

} // namespace Panda