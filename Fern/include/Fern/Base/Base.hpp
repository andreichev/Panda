#pragma once

#include <Foundation/Foundation.hpp>
#include <float.h>

namespace Fern {

struct Vec2 {
    union {
        float x, width;
    };
    union {
        float y, height;
    };

    Vec2()
        : x(0)
        , y(0) {}

    Vec2(float x, float y)
        : x(x)
        , y(y) {}

    bool operator==(const Vec2 &l) {
        return l.x == x && l.y == y;
    }

    bool operator!=(const Vec2 &l) {
        return !(*this == l);
    }
};

struct Vec3 {
    union {
        float x, r, width;
    };
    union {
        float y, g, height;
    };
    union {
        float z, b;
    };

    Vec3()
        : x(0)
        , y(0)
        , z(0) {}

    Vec3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z) {}
};

struct Vec4 {
    union {
        float x, r, width;
    };
    union {
        float y, g, height;
    };
    union {
        float z, b;
    };
    union {
        float w, a;
    };

    Vec4()
        : x(0)
        , y(0)
        , z(0)
        , w(0) {}

    Vec4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w) {}

    bool operator==(const Vec4 &other) const {
        return Foundation::epsilonEqual(x, other.x, FLT_EPSILON) &&
               Foundation::epsilonEqual(y, other.y, FLT_EPSILON) &&
               Foundation::epsilonEqual(z, other.z, FLT_EPSILON) &&
               Foundation::epsilonEqual(w, other.w, FLT_EPSILON);
    }
};

using Size = Vec2;
using Color = Vec4;
using Quat = Vec4;

struct Rect {
    Vec2 origin;
    Vec2 size;

    Rect() {}

    Rect(float x, float y, float width, float height)
        : origin(x, y)
        , size(width, height) {}
};

enum class WindowState {
    WindowStateNone = 0,
    WindowStateNormal,
    WindowStateMaximized,
    WindowStateMinimized,
    WindowStateFullScreen
};

enum class DrawingContextType {
    DrawingContextTypeNone = 0,
    DrawingContextTypeOpenGL,
    DrawingContextTypeD3D,
    DrawingContextTypeMetal,
    DrawingContextTypeVulkan,
};

} // namespace Fern
