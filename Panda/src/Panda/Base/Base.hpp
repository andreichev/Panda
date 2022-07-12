//
// Created by Admin on 22.01.2022.
//

#pragma once

// joke from swift (apple) or js
#define var auto
#define let const auto

namespace Panda {

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Unique<T> makeUnique(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
constexpr Unique<T> createUnique(T *ptr) {
    return std::unique_ptr<T>(ptr);
}

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Shared<T> makeShared(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
constexpr Shared<T> createShared(T *ptr) {
    return std::shared_ptr<T>(ptr);
}

template<typename T>
using Weak = std::weak_ptr<T>;

template<typename T, typename... Args>
constexpr Weak<T> makeWeak(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
constexpr Weak<T> createWeak(T *ptr) {
    return std::weak_ptr<T>(ptr);
}

#define PND_STATIC_ASSERT(_condition, ...) static_assert(_condition, "" __VA_ARGS__)

#define PND_ASSERT(_condition, _format, ...)                                                                                               \
    if (_condition == false) {                                                                                                             \
        PND_CRITICAL("CHECK " _format, ##__VA_ARGS__);                                                                                     \
        assert(false);                                                                                                                     \
    }

} // namespace Panda
