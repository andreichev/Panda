#pragma once
// #pragma clang diagnostic push
// #pragma ide diagnostic ignored "google-explicit-constructor"

// "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer
// used to "uniquely" identify information. In Vivo, even though we use the term
// GUID and UUID, at the moment we're simply using a randomly generated 64-bit
// integer, as the possibility of a clash is low enough for now.
// This may change in the future.

#include <cstdint>
#include <string>

class UUID final {
public:
    UUID();
    UUID(uint32_t uuid);
    UUID(const UUID &other);

    operator uint32_t() {
        return m_UUID;
    }
    operator const uint32_t() const {
        return m_UUID;
    }

private:
    uint32_t m_UUID;
};

namespace std {

template<>
struct hash<UUID> {
    std::size_t operator()(const UUID &uuid) const {
        // uuid is already a randomly generated number, and is suitable as a hash key as-is.
        // this may change in the future, in which case return hash<uint64_t>{}(uuid); might be more
        // appropriate
        return uuid;
    }
};

} // namespace std