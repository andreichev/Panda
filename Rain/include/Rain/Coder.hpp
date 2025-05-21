#pragma once

#include "TypeInfo.hpp"
#include "UUID.hpp"

#include <istream>
#include <ostream>
#include <filesystem>

namespace Rain {

class Codable;

class Encoder {
public:
    virtual ~Encoder() = default;

    template<typename T>
    void encode(std::ostream &stream, const T &data);

    template<typename T>
        requires std::derived_from<T, Codable>
    void encode(const char *key, const T &data);

    template<typename T>
        requires std::is_enum_v<T>
    void encode(const char *key, const T &data);

    /// General encode
    virtual void encode(std::ostream &, void *data, TypeInfo info) = 0;
    /// Encode null
    virtual void encodeNull(const char *key) = 0;
    /// Begin encoding object
    virtual void beginObject(const char *key) = 0;
    /// Encode bool
    virtual void encode(const char *key, const bool &data) = 0;
    /// Encode int
    virtual void encode(const char *key, const int &data) = 0;
    /// Encode uint
    virtual void encode(const char *key, const uint32_t &data) = 0;
    /// Encode int64
    virtual void encode(const char *key, const int64_t &data) = 0;
    /// Encode uint64
    virtual void encode(const char *key, const uint64_t &data) = 0;
    /// Encode float
    virtual void encode(const char *key, const float &data) = 0;
    /// Encode double
    virtual void encode(const char *key, const double &data) = 0;
    /// Encode string
    virtual void encode(const char *key, const std::string &data) = 0;
    /// Encode const char*
    virtual void encode(const char *key, const char *&data) = 0;
    /// Encode uuid
    virtual void encode(const char *key, const UUID &data) = 0;
    /// End encoding object
    virtual void endObject() = 0;
    /// Begin encoding array
    virtual void beginArray(const char *key) = 0;
    /// End encoding array
    virtual void endArray() = 0;
};

class Decoder {
public:
    virtual ~Decoder() = default;

    template<typename T>
    bool decode(std::istream &stream, T &data);

    template<typename T>
        requires std::derived_from<T, Codable>
    bool decode(const char *key, T &data);

    template<typename T>
        requires std::is_enum_v<T>
    bool decode(const char *key, T &data);

    /// General decode
    virtual bool decode(std::istream &, void *data, TypeInfo info) = 0;
    /// Begin decoding object
    virtual bool beginObject(const char *key) = 0;
    /// Decode bool
    virtual bool decode(const char *key, bool &data) = 0;
    /// Decode int
    virtual bool decode(const char *key, int &data) = 0;
    /// Decode uint
    virtual bool decode(const char *key, uint32_t &data) = 0;
    /// Decode int64
    virtual bool decode(const char *key, int64_t &data) = 0;
    /// Decode uint64
    virtual bool decode(const char *key, uint64_t &data) = 0;
    /// Decode float
    virtual bool decode(const char *key, float &data) = 0;
    /// Decode double
    virtual bool decode(const char *key, double &data) = 0;
    /// Decode string
    virtual bool decode(const char *key, std::string &data) = 0;
    /// Decode uuid
    virtual bool decode(const char *key, UUID &data) = 0;
    /// End decoding object
    virtual void endObject() = 0;
    /// Begin decoding array
    virtual bool beginArray(const char *key) = 0;
    /// Check if array has current element
    virtual bool arrayHasElement() = 0;
    /// Go to next array element
    virtual void arrayNext() = 0;
    /// End decoding array
    virtual void endArray() = 0;
};

} // namespace Rain