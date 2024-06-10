#pragma once

#include <cstdlib>
#include <type_traits>

namespace Rain {

// --------------------------------
// ---------- HANDLE --------------
// --------------------------------

#define RAIN_INVALID_HANDLE UINT16_MAX

#define RAIN_HANDLE(name) using name = uint32_t;

RAIN_HANDLE(TypeId)
RAIN_HANDLE(FieldId)

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

inline const void *addOffset(const void *data, size_t offset) {
    return static_cast<const uint8_t *>(data) + offset;
}

} // namespace Rain