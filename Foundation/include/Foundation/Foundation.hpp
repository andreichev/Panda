//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Logger.hpp"
#include "Assert.hpp"
#include "Allocator.hpp"

// joke from swift (apple) or js
#define var auto
#define let const auto

namespace Foundation {

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Unique<T> makeUnique(Args &&...args) {
    AllocatorI *alloc = getAllocator();
    T *ptr = NEW(alloc, T)(std::forward<Args>(args)...);
    auto deleter = [alloc](T *ptr) { FREE(alloc, ptr); };
    return std::unique_ptr<T>(ptr, deleter);
}

template<typename T>
constexpr Unique<T> createUnique(T *ptr) {
    AllocatorI *alloc = getAllocator();
    auto deleter = [alloc](T *ptr) { FREE(alloc, ptr); };
    return std::unique_ptr<T>(ptr, deleter);
}

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
constexpr Shared<T> createShared(T *ptr) {
    AllocatorI *alloc = getAllocator();
    auto deleter = [alloc](T *ptr) { FREE(alloc, ptr); };
    return std::shared_ptr<T>(ptr, deleter);
}

template<typename T, typename... Args>
Shared<T> makeShared(Args &&...args) {
    AllocatorI *alloc = getAllocator();
    T *ptr = NEW(alloc, T)(std::forward<Args>(args)...);
    auto deleter = [alloc](T *ptr) { FREE(alloc, ptr); };
    return std::shared_ptr<T>(ptr, deleter);
}

template<typename T>
using Weak = std::weak_ptr<T>;

template<typename T>
constexpr Weak<T> createWeak(T *ptr) {
    AllocatorI *alloc = getAllocator();
    auto deleter = [alloc](T *ptr) { FREE(alloc, ptr); };
    return std::weak_ptr<T>(ptr);
}

template<typename T, typename... Args>
Shared<T> makeWeak(Args &&...args) {
    AllocatorI *alloc = getAllocator();
    T *ptr = NEW(alloc, T)(std::forward<Args>(args)...);
    auto deleter = [alloc](T *ptr) { PND_FREE(alloc, ptr); };
    return std::weak_ptr<T>(ptr, deleter);
}

} // namespace Foundation
