#pragma once

#include "Config.hpp"

#include <cstdlib>

namespace Miren {

enum class UniformDataType { Int, Mat4 };

using ShaderHandle = uint16_t;
using TextureHandle = uint16_t;
using IndexBufferHandle = uint16_t;
using VertexBufferHandle = uint16_t;
using VertexLayoutHandle = uint16_t;

struct TransientIndexBuffer {
    uint8_t *data;
    uint32_t size;
    uint32_t startIndex;
    IndexBufferHandle handle;
    bool isIndex16;
};

struct TransientVertexBuffer {
    uint8_t *data;
    uint32_t size;
    uint32_t startVertex;
    uint16_t stride;
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
};

struct Size {
    float width;
    float height;

    Size()
        : width(0)
        , height(0) {}

    Size(float width, float height)
        : width(width)
        , height(height) {}

    inline bool isZero() {
        return width == 0 && height == 0;
    }
};

struct Point {
    float x;
    float y;

    Point()
        : x(0)
        , y(0) {}

    Point(float x, float y)
        : x(x)
        , y(y) {}

    inline bool isZero() {
        return x == 0 && y == 0;
    }
};

struct Rect {
    Point origin;
    Size size;

    Rect()
        : origin()
        , size() {}

    Rect(float x, float y, float width, float height)
        : origin(x, y)
        , size(width, height) {}

    inline static Rect zero() {
        return Rect();
    }

    inline bool isZero() {
        return origin.isZero() && size.isZero();
    }
};

} // namespace Miren