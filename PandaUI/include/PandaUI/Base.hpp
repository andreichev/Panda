#pragma once

#include <Foundation/Foundation.hpp>
#include <glm/glm.hpp>

namespace PandaUI {

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

    Point(glm::vec2 p)
        : x(p.x)
        , y(p.y) {}

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

    Rect(int x, int y, int width, int height)
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

    Color(glm::vec4 c)
        : r(c.x)
        , g(c.y)
        , b(c.z)
        , a(c.w) {}

    Color(float r, float g, float b, float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a) {}

    Color(uint32_t rgba) {
        uint8_t _r = rgba >> 24;
        uint8_t _g = rgba >> 16;
        uint8_t _b = rgba >> 8;
        uint8_t _a = rgba >> 0;
        r = (_r) / 255.f;
        g = (_g) / 255.f;
        b = (_b) / 255.f;
        a = (_a) / 255.f;
    }
};

} // namespace PandaUI