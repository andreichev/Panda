#pragma once

#include "TypeInfo.hpp"
#include "Coder.hpp"
#include "TypeName.hpp"

#include <unordered_map>
#include <string>
#include <optional>
#include <utility>

namespace Rain {

class TypeRegistry;
class Codable;

TypeRegistry *getTypeRegistry();

template<typename T>
struct TypeEncoder;

template<typename T>
struct TypeDecoder;

template<typename T>
concept PrimitiveEncoder =
    requires(const char *key, Encoder *encoder, const TypeInfo &info, const T &data) {
        TypeEncoder<T>::encode(key, encoder, info, data);
    };

template<typename T>
concept CustomEncoder =
    requires(const char *key, Encoder *encoder, const T &data) { T::encode(key, encoder, data); };

template<typename T>
concept PrimitiveDecoder =
    requires(const char *key, Decoder *decoder, const TypeInfo &info, T &data) {
        TypeDecoder<T>::decode(key, decoder, info, data);
    };

template<typename T>
concept CustomDecoder = requires(const char *key, Decoder *decoder, T &data) {
    typeid(T::decode(key, decoder, data)) == typeid(bool);
};

class TypeRegistry {
private:
    template<typename T>
    static auto addFieldsIfHas(TypeInfo &) -> std::enable_if_t<!hasFields<T>> {}

    template<typename T>
    static auto addFieldsIfHas(TypeInfo &info) -> std::enable_if_t<hasFields<T>> {
        info.fields = StripType<T>::getFields();
    }

public:
    template<typename T>
    constexpr void fillCodingFunc(TypeInfo &info) {
        if constexpr (CustomEncoder<T>) {
            info.encoderFunc =
                [](const char *key, Encoder *encoder, const TypeInfo &info, const void *data) {
                    const T *ptr = static_cast<const T *>(data);
                    T::encode(key, encoder, *ptr);
                };
        } else if constexpr (PrimitiveEncoder<T>) {
            info.encoderFunc =
                [](const char *key, Encoder *encoder, const TypeInfo &info, const void *data) {
                    const T *ptr = static_cast<const T *>(data);
                    TypeEncoder<T>::encode(key, encoder, info, *ptr);
                };
        } else {
            info.encoderFunc =
                [](const char *key, Encoder *encoder, const TypeInfo &info, const void *data) {
                    static_assert(std::is_base_of<Codable, T>());
                    encoder->beginObject(key);
                    for (auto &field : info.fields) {
                        auto memberInfo = getTypeRegistry()->findInfo(field.typeId);
                        memberInfo.encoderFunc(
                            field.name, encoder, memberInfo, addOffset(data, field.offset)
                        );
                    }
                    encoder->endObject();
                };
        }
        if constexpr (CustomDecoder<T>) {
            info.decoderFunc =
                [](const char *key, Decoder *decoder, const TypeInfo &info, void *data) {
                    T *ptr = static_cast<T *>(data);
                    return T::decode(key, decoder, *ptr);
                };
        } else if constexpr (PrimitiveDecoder<T>) {
            info.decoderFunc =
                [](const char *key, Decoder *decoder, const TypeInfo &info, void *data) {
                    T *ptr = static_cast<T *>(data);
                    return TypeDecoder<T>::decode(key, decoder, info, *ptr);
                };
        } else {
            info.decoderFunc =
                [](const char *key, Decoder *decoder, const TypeInfo &info, void *data) {
                    static_assert(std::is_base_of<Codable, T>());
                    if (!decoder->beginObject(key)) {
                        return false;
                    }
                    for (auto &field : info.fields) {
                        auto memberInfo = getTypeRegistry()->findInfo(field.typeId);
                        if (!memberInfo.decoderFunc(
                                field.name, decoder, memberInfo, addOffset(data, field.offset)
                            )) {
                            return false;
                        }
                    }
                    decoder->endObject();
                    return true;
                };
        }
    }

    template<typename T>
    TypeInfo makeTypeInfo(TypeId id, const std::string &name) {
        // std::cout << "CREATE TYPE INFO: " << name << std::endl;
        TypeInfo info(id, name);
        info.size = sizeof(T);
        info.align = alignof(T);
        fillCodingFunc<T>(info);
        return info;
    }

    template<typename T>
    TypeInfo findOrCreateType() {
        std::string_view name = typeName<T>();
        const TypeId hash = std::hash<std::string_view>()(name);
        const auto it = m_data.find(hash);
        if (it == m_data.end()) {
            std::string str(name);
            TypeInfo typeInfo = makeTypeInfo<T>(hash, str);
            addFieldsIfHas<T>(typeInfo);
            m_data.emplace(hash, typeInfo);
            return typeInfo;
        }
        TypeInfo a = it->second;
        return a;
    }

    TypeInfo &findInfo(TypeId id) {
        const auto it = m_data.find(id);
        // assert(it != data.end());
        return it->second;
    }

    template<typename FieldType>
    FieldInfo makeFieldInfo(const char *fieldName, uint32_t offset) {
        TypeInfo fieldTypeInfo = findOrCreateType<FieldType>();
        TypeId fieldTypeId = fieldTypeInfo.id;

        using TypeRemovedExtents = std::remove_all_extents_t<FieldType>;
        using TypeRemovedRefs = std::remove_reference_t<TypeRemovedExtents>;
        using TypeRemovedPtrs = RemoveAllPointers<TypeRemovedRefs>;
        constexpr bool isRef = std::is_reference_v<FieldType>;
        constexpr bool isRValRef = std::is_rvalue_reference_v<FieldType>;
        constexpr bool isConst = std::is_const_v<TypeRemovedPtrs>;
        constexpr bool isVolatile = std::is_volatile_v<TypeRemovedPtrs>;

        FieldInfo field(fieldTypeId, fieldName, offset, alignof(FieldType));

        if constexpr (isConst) {
            field.setConstFlag();
        }
        if constexpr (isVolatile) {
            field.setVolatileFlag();
        }
        if constexpr (isRef) {
            field.setReferenceFlag();
        }
        if constexpr (isRValRef) {
            field.setRValReferenceFlag();
        }

        return field;
    }

private:
    std::unordered_map<TypeId, TypeInfo> m_data;
};

// --------------------------------------------------------
// ---------- GENERAL RAIN CODING FUNCTION --------------
// --------------------------------------------------------

template<typename T>
void Encoder::encode(std::ostream &stream, const T &_data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    T &data = const_cast<T &>(_data);
    encode(stream, &data, info);
}

/// Encode object function. Not primitive, object which is child of Codable
template<typename T>
    requires std::derived_from<T, Codable>
void Encoder::encode(const char *key, const T &_data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    T &data = const_cast<T &>(_data);
    info.encoderFunc(key, this, info, &data);
}

/// Encode enum function
template<typename T>
    requires std::is_enum_v<T>
void Encoder::encode(const char *key, const T &data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    TypeEncoder<T>::encode(key, this, info, data);
}

template<typename T>
bool Decoder::decode(std::istream &stream, T &data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    return decode(stream, &data, info);
}

/// Decode object function. Not primitive, object which is child of Codable
template<typename T>
    requires std::derived_from<T, Codable>
bool Decoder::decode(const char *key, T &data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    return info.decoderFunc(key, this, info, &data);
}

/// Decode enum function
template<typename T>
    requires std::is_enum_v<T>
bool Decoder::decode(const char *key, T &data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    return TypeDecoder<T>::decode(key, this, info, data);
}

// ------------------------------------------------------------
// ---------- PRIMITIVES DECODING IMPLEMENTATION --------------
// ------------------------------------------------------------

/** ENUM */
template<typename T>
    requires std::is_enum_v<T>
struct TypeDecoder<T> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, T &_data) {
        uint32_t data;
        if (!decoder->decode(key, data)) {
            return false;
        }
        _data = static_cast<T>(data);
        return true;
    }
};

/** FLOAT */
template<>
struct TypeDecoder<float> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, float &data) {
        return decoder->decode(key, data);
    }
};

/** DOUBLE */
template<>
struct TypeDecoder<double> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, double &data) {
        return decoder->decode(key, data);
    }
};

/** CONST CHAR* */
/// IMPORTANT! Memory for const char* should be prepared. Should not be just a raw pointer
template<>
struct TypeDecoder<const char *> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, const char *&data) {
        if (!data) {
            return true;
        }
        std::string buffer;
        if (!decoder->decode(key, buffer)) {
            return false;
        }
        memccpy((void *)data, buffer.c_str(), 0, buffer.size());
        return true;
    }
};

/** CONST CHAR ARRAY */
template<size_t N>
struct TypeDecoder<const char[N]> {
    static bool
    decode(const char *key, Decoder *decoder, const TypeInfo &info, const char data[N]) {
        return TypeDecoder<const char *>::decode(key, decoder, info, data);
    }
};

/** STRING */
template<typename Elem, typename Traits, typename Alloc>
struct TypeDecoder<std::basic_string<Elem, Traits, Alloc>> {
    static bool decode(
        const char *key,
        Decoder *decoder,
        const TypeInfo &info,
        std::basic_string<Elem, Traits, Alloc> &data
    ) {
        return decoder->decode(key, data);
    }
};

/** PATH */
template<>
struct TypeDecoder<std::filesystem::path> {
    static bool
    decode(const char *key, Decoder *decoder, const TypeInfo &info, std::filesystem::path &data) {
        std::string path;
        if (!decoder->decode(key, path)) {
            return false;
        }
        data = path;
        return true;
    }
};

/** INT */
template<>
struct TypeDecoder<int> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, int &data) {
        return decoder->decode(key, data);
    }
};

/** UINT */
template<>
struct TypeDecoder<uint32_t> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, uint32_t &data) {
        return decoder->decode(key, data);
    }
};

/** INT64 */
template<>
struct TypeDecoder<int64_t> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, int64_t &data) {
        return decoder->decode(key, data);
    }
};

/** UINT64 */
template<>
struct TypeDecoder<uint64_t> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, uint64_t &data) {
        return decoder->decode(key, data);
    }
};

/** UUID */
template<>
struct TypeDecoder<UUID> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, UUID &data) {
        return decoder->decode(key, data);
    }
};

/** BOOL */
template<>
struct TypeDecoder<bool> {
    static bool decode(const char *key, Decoder *decoder, const TypeInfo &info, bool &data) {
        return decoder->decode(key, data);
    }
};

/** OPTIONAL */
template<typename T>
struct TypeDecoder<std::optional<T>> {
    static bool
    decode(const char *key, Decoder *decoder, const TypeInfo &_info, std::optional<T> &data) {
        TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
        T value;
        if (info.decoderFunc(key, decoder, info, &value)) {
            data = value;
        } else {
            data = {};
        }
        return true;
    }
};

/** VECTOR */
template<typename T, typename Alloc>
struct TypeDecoder<std::vector<T, Alloc>> {
    static bool
    decode(const char *key, Decoder *decoder, const TypeInfo &info, std::vector<T, Alloc> &data) {
        data.clear();
        if (!decoder->beginArray(key)) {
            return false;
        }
        auto memberInfo = getTypeRegistry()->findOrCreateType<T>();
        while (decoder->arrayHasElement()) {
            T value;
            if (memberInfo.decoderFunc(nullptr, decoder, memberInfo, &value)) {
                data.push_back(value);
            }
            decoder->arrayNext();
        }
        decoder->endArray();
        return true;
    }
};

/** UNORDERED_MAP */
template<typename V, typename Alloc>
struct TypeDecoder<std::unordered_map<std::string, V, Alloc>> {
    static bool decode(
        const char *key,
        Decoder *decoder,
        const TypeInfo &info,
        std::unordered_map<std::string, V, Alloc> &data
    ) {
        data.clear();
        if (!decoder->beginObject(key)) {
            return false;
        }
        auto memberInfo = getTypeRegistry()->findOrCreateType<V>();
        std::vector<std::string> keys = decoder->getMemberKeys();
        for (auto &key : keys) {
            V value;
            if (!memberInfo.decoderFunc(key.c_str(), decoder, memberInfo, &value)) {
                return false;
            }
            data[key] = value;
        }
        decoder->endObject();
        return true;
    }
};

// ------------------------------------------------------------
// ---------- PRIMITIVES ENCODING IMPLEMENTATION --------------
// ------------------------------------------------------------

/** ENUM */
template<typename T>
    requires std::is_enum_v<T>
struct TypeEncoder<T> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, const T &_data) {
        uint32_t data = static_cast<uint32_t>(_data);
        encoder->encode(key, data);
    }
};

/** FLOAT */
template<>
struct TypeEncoder<float> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, const float &data) {
        encoder->encode(key, data);
    }
};

/** DOUBLE */
template<>
struct TypeEncoder<double> {
    static void
    encode(const char *key, Encoder *encoder, const TypeInfo &info, const double &data) {
        encoder->encode(key, data);
    }
};

/** CONST CHAR* */
template<>
struct TypeEncoder<const char *> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, const char *&data) {
        if (!data) {
            data = "";
        }
        encoder->encode(key, data);
    }
};

/** CONST CHAR ARRAY */
template<size_t N>
struct TypeEncoder<const char[N]> {
    static void
    encode(const char *key, Encoder *encoder, const TypeInfo &info, const char data[N]) {
        return TypeEncoder<const char *>::encode(key, encoder, info, data);
    }
};

/** STRING */
template<typename Elem, typename Traits, typename Alloc>
struct TypeEncoder<std::basic_string<Elem, Traits, Alloc>> {
    static void encode(
        const char *key,
        Encoder *encoder,
        const TypeInfo &info,
        const std::basic_string<Elem, Traits, Alloc> &data
    ) {
        encoder->encode(key, data);
    }
};

/** PATH */
template<>
struct TypeEncoder<std::filesystem::path> {
    static void encode(
        const char *key, Encoder *encoder, const TypeInfo &info, const std::filesystem::path &data
    ) {
        encoder->encode(key, data.string());
    }
};

/** INT */
template<>
struct TypeEncoder<int> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, const int &data) {
        encoder->encode(key, data);
    }
};

/** UINT */
template<>
struct TypeEncoder<uint32_t> {
    static void
    encode(const char *key, Encoder *encoder, const TypeInfo &info, const uint32_t &data) {
        encoder->encode(key, data);
    }
};

/** INT64 */
template<>
struct TypeEncoder<int64_t> {
    static void
    encode(const char *key, Encoder *encoder, const TypeInfo &info, const int64_t &data) {
        encoder->encode(key, data);
    }
};

/** UINT64 */
template<>
struct TypeEncoder<uint64_t> {
    static void
    encode(const char *key, Encoder *encoder, const TypeInfo &info, const uint64_t &data) {
        encoder->encode(key, data);
    }
};

/** UUID */
template<>
struct TypeEncoder<UUID> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, const UUID &data) {
        encoder->encode(key, data);
    }
};

/** BOOL */
template<>
struct TypeEncoder<bool> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, const bool &data) {
        encoder->encode(key, data);
    }
};

/** OPTIONAL */
template<typename T>
struct TypeEncoder<std::optional<T>> {
    static void
    encode(const char *key, Encoder *encoder, const TypeInfo &info, const std::optional<T> &data) {
        if (data.has_value()) {
            TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
            info.encoderFunc(key, encoder, info, &data);
        } else {
            encoder->encodeNull(key);
        }
    }
};

/** VECTOR */
template<typename T, typename Alloc>
struct TypeEncoder<std::vector<T, Alloc>> {
    static void encode(
        const char *key, Encoder *encoder, const TypeInfo &info, const std::vector<T, Alloc> &data
    ) {
        encoder->beginArray(key);
        auto memberInfo = getTypeRegistry()->findOrCreateType<T>();
        for (auto &item : data) {
            memberInfo.encoderFunc(nullptr, encoder, memberInfo, &item);
        }
        encoder->endArray();
    }
};

/** UNORDERED_MAP */
template<typename V, typename Alloc>
struct TypeEncoder<std::unordered_map<std::string, V, Alloc>> {
    static void encode(
        const char *key,
        Encoder *encoder,
        const TypeInfo &info,
        const std::unordered_map<std::string, V, Alloc> &data
    ) {
        encoder->beginObject(key);
        auto memberInfo = getTypeRegistry()->findOrCreateType<V>();
        for (auto &item : data) {
            memberInfo.encoderFunc(item.first.c_str(), encoder, memberInfo, &item.second);
        }
        encoder->endObject();
    }
};

} // namespace Rain
