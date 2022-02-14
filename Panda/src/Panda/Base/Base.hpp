//
// Created by Admin on 22.01.2022.
//

#pragma once

// joke from swift (apple) or js
#define var auto
#define let const auto
#define in :
// joke from C# and JAVA
#define interface class

namespace Panda {

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename... Args>
constexpr Unique<T> createUnique(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Shared = std::shared_ptr<T>;
template<typename T, typename... Args>
constexpr Shared<T> createShared(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Weak = std::weak_ptr<T>;
template<typename T, typename... Args>
constexpr Weak<T> createWeak(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace Panda