#pragma once

#include <cstdint>
#include <type_traits>

namespace Panda {

using UUID = uint32_t;
using ScriptHandle = uint32_t;

// --------------------------------
// -------- TYPE UTILS ------------
// --------------------------------

template<typename T>
struct RemoveAllPointersHelper {
    using Type = T;
};

template<typename T>
struct RemoveAllPointersHelper<T *> {
    using Type = typename RemoveAllPointersHelper<T>::Type;
};

template<typename T>
using RemoveAllPointers = typename RemoveAllPointersHelper<T>::Type;

template<typename T>
struct StripTypeHelper {
    using Type = std::remove_cvref_t<
        RemoveAllPointers<std::remove_reference_t<std::remove_all_extents_t<T>>>>;
};

template<typename T>
using StripType = typename StripTypeHelper<T>::Type;

template<typename T>
concept hasFields = requires(T) { StripType<T>::getFields(); };

template<typename T, typename U>
constexpr std::size_t offsetOf(U T::*member) {
    return reinterpret_cast<std::size_t>(&(reinterpret_cast<T const volatile *>(0)->*member));
}

inline void *addOffset(void *data, size_t offset) {
    return static_cast<uint8_t *>(data) + offset;
}

} // namespace Panda