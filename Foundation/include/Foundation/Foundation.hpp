//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Logger.hpp"
#include "Assert.hpp"
#include "Memory.hpp"
#include "Allocator.hpp"
#include "Vector.hpp"

namespace Foundation {

#define PND_DISCARD(x) static_cast<void>(x)

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
constexpr Shared<T> createShared(T *ptr) {
    AllocatorI *alloc = getAllocator();
    auto deleter = [alloc](T *ptr) { F_DELETE(alloc, ptr); };
    return std::shared_ptr<T>(ptr, deleter);
}

template<typename T1, typename T2>
inline Shared<T1> SharedCast(const Shared<T2> &r) {
    return std::dynamic_pointer_cast<T1>(r);
}

template<typename T, typename... Args>
constexpr Shared<T> makeShared(Args &&...args) {
    AllocatorI *alloc = getAllocator();
    T *ptr = F_NEW(alloc, T)(std::forward<Args>(args)...);
    auto deleter = [alloc](T *ptr) { F_DELETE(alloc, ptr); };
    return std::shared_ptr<T>(ptr, deleter);
}

template<typename T>
using Weak = std::weak_ptr<T>;

template<typename T>
constexpr Weak<T> createWeak(T *ptr) {
    AllocatorI *alloc = getAllocator();
    auto deleter = [alloc](T *ptr) { F_FREE(alloc, ptr); };
    return std::weak_ptr<T>(ptr);
}

template<typename T, typename... Args>
constexpr Shared<T> makeWeak(Args &&...args) {
    AllocatorI *alloc = getAllocator();
    T *ptr = F_NEW(alloc, T)(std::forward<Args>(args)...);
    auto deleter = [alloc](T *ptr) { F_DELETE(alloc, ptr); };
    return std::weak_ptr<T>(ptr, deleter);
}

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

template<typename Ty>
inline constexpr Ty clamp(const Ty &_a, const Ty &_min, const Ty &_max) {
    return max(min(_a, _max), _min);
}

inline int32_t strCmp(const char *lhs, const char *rhs, int32_t max) {
    for (; max > 0 && *lhs == *rhs; ++lhs, ++rhs, --max) {
        if (*lhs == '\0' || *rhs == '\0') { break; }
    }

    if (0 == max) { return 0; }

    return *lhs - *rhs;
}

inline bool epsilonEqual(float const &x, float const &y, float const &epsilon) {
    return abs(x - y) < epsilon;
}

} // namespace Foundation
