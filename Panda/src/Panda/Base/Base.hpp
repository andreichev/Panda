#pragma once

namespace Panda {

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

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color()
        : r(1)
        , g(0)
        , b(0)
        , a(1) {}

    Color(float r, float g, float b, float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a) {}
};

} // namespace Panda