#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>
#include <Rain/Rain.hpp>

namespace Panda {

template<size_t L, typename T>
struct Vec;

template<typename T>
struct Vec<2, T> : public Rain::Codable {
    union {
        T x, width;
    };
    union {
        T y, height;
    };

    Vec()
        : x(0)
        , y(0) {}

    Vec(glm::vec2 c)
        : x(c.x)
        , y(c.y) {}

    Vec(T x, T y)
        : x(x)
        , y(y) {}

    Vec(const ImVec2 &vec)
        : x(vec.x)
        , y(vec.y) {}

    bool operator==(const Vec &l) {
        return l.x == x && l.y == y;
    }

    bool operator!=(const Vec &l) {
        return !(*this == l);
    }

    operator ImVec2() {
        return {x, y};
    }

    operator glm::vec2() {
        return {x, y};
    }

    bool isZero() {
        return x == 0 && y == 0;
    }

    RAIN_FIELDS_BEGIN(Vec)
    RAIN_FIELD(x)
    RAIN_FIELD(y)
    RAIN_FIELDS_END
};

using Vec2 = Vec<2, float>;
using UVec2 = Vec<2, uint32_t>;
using IVec2 = Vec<2, int32_t>;

template<typename T>
struct Vec<3, T> : public Rain::Codable {
    union {
        T x, r, width;
    };
    union {
        T y, g, height;
    };
    union {
        T z, b;
    };

    Vec()
        : x(0)
        , y(0)
        , z(0) {}

    Vec(glm::vec3 c)
        : x(c.x)
        , y(c.y)
        , z(c.z) {}

    Vec(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z) {}

    operator glm::vec3() {
        return {x, y, z};
    }

    RAIN_FIELDS_BEGIN(Vec)
    RAIN_FIELD(x)
    RAIN_FIELD(y)
    RAIN_FIELD(z)
    RAIN_FIELDS_END
};

using Vec3 = Vec<3, float>;
using UVec3 = Vec<3, uint32_t>;
using IVec3 = Vec<3, int32_t>;

template<typename T>
struct Vec<4, T> : public Rain::Codable {
    union {
        T x, r, width;
    };
    union {
        T y, g, height;
    };
    union {
        T z, b;
    };
    union {
        T w, a;
    };

    Vec()
        : x(0)
        , y(0)
        , z(0)
        , w(0) {}

    Vec(glm::vec4 c)
        : x(c.x)
        , y(c.y)
        , z(c.z)
        , w(c.w) {}

    Vec(glm::quat c)
        : x(c.x)
        , y(c.y)
        , z(c.z)
        , w(c.w) {}

    Vec(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w) {}

    operator glm::vec4() {
        return {x, y, z, w};
    }

    operator glm::quat() {
        glm::quat q;
        q.x = x;
        q.y = y;
        q.z = z;
        q.w = w;
        return q;
    }

    bool operator==(const Vec &other) const {
        return glm::epsilonEqual(x, other.x, FLT_EPSILON) &&
               glm::epsilonEqual(y, other.y, FLT_EPSILON) &&
               glm::epsilonEqual(z, other.z, FLT_EPSILON) &&
               glm::epsilonEqual(w, other.w, FLT_EPSILON);
    }

    RAIN_FIELDS_BEGIN(Vec)
    RAIN_FIELD(x)
    RAIN_FIELD(y)
    RAIN_FIELD(z)
    RAIN_FIELD(w)
    RAIN_FIELDS_END
};

using Vec4 = Vec<4, float>;
using UVec4 = Vec<4, uint32_t>;
using IVec4 = Vec<4, int32_t>;

using Size = Vec2;
using USize = UVec2;
using ISize = IVec2;
using Color = Vec4;
using Quat = Vec4;

template<typename T = float>
struct TRect : public Rain::Codable {
    Vec<2, T> origin;
    Vec<2, T> size;

    TRect() {}

    TRect(T x, T y, T width, T height)
        : origin(x, y)
        , size(width, height) {}

    RAIN_FIELDS_BEGIN(TRect)
    RAIN_FIELD(origin)
    RAIN_FIELD(size)
    RAIN_FIELDS_END
};

using Rect = TRect<float>;
using URect = TRect<uint32_t>;
using IRect = TRect<int32_t>;

} // namespace Panda