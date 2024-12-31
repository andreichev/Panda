#pragma once

#include <cstdint>

namespace Bamboo {

using EntityHandle = uint32_t;
using TextureHandle = uint32_t;

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

    static Vec2 zero() {
        return Vec2();
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
};

using Size = Vec2;
using Color = Vec4;
using Quat = Vec4;

template<typename Ty>
inline void swap(Ty &_a, Ty &_b) {
    Ty tmp = _a;
    _a = _b;
    _b = tmp;
}

template<typename Ty>
inline constexpr Ty min(const Ty &_a, const Ty &_b) {
    return _a < _b ? _a : _b;
}

template<typename Ty>
inline constexpr Ty max(const Ty &_a, const Ty &_b) {
    return _a > _b ? _a : _b;
}

inline int32_t strCmp(const char *lhs, const char *rhs, int32_t max) {
    for (; max > 0 && *lhs == *rhs; ++lhs, ++rhs, --max) {
        if (*lhs == '\0' || *rhs == '\0') {
            break;
        }
    }

    if (0 == max) {
        return 0;
    }

    return *lhs - *rhs;
}

} // namespace Bamboo