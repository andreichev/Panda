#pragma once

#include <glm/glm.hpp>
#include <imgui.h>

namespace Panda {

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

    Vec2(glm::vec2 c)
        : x(c.x)
        , y(c.y) {}

    Vec2(float x, float y)
        : x(x)
        , y(y) {}

    Vec2(const ImVec2 &vec)
        : x(vec.x)
        , y(vec.y) {}

    bool operator==(const Vec2 &l) {
        return l.x == x && l.y == y;
    }

    bool operator!=(const Vec2 &l) {
        return !(*this == l);
    }

    operator ImVec2() {
        return {x, y};
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

    Vec3(glm::vec3 c)
        : x(c.x)
        , y(c.y)
        , z(c.z) {}

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

    Vec4(glm::vec4 c)
        : x(c.x)
        , y(c.y)
        , z(c.z)
        , w(c.w) {}

    Vec4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w) {}
};

using id_t = uint64_t;
using Size = Vec2;
using Color = Vec4;

} // namespace Panda