//
// Created by Admin on 22.01.2022.
//

#pragma once

namespace Panda {

struct FSize {
    float width;
    float height;

    FSize()
        : width(0)
        , height(0) {}

    FSize(float width, float height)
        : width(width)
        , height(height) {}

    inline bool isZero() {
        return width == 0 && height == 0;
    }
};

struct UISize {
    uint32_t width;
    uint32_t height;

    UISize()
        : width(0)
        , height(0) {}

    UISize(uint32_t width, uint32_t height)
        : width(width)
        , height(height) {}

    inline bool isZero() {
        return width == 0 && height == 0;
    }
};

} // namespace Panda