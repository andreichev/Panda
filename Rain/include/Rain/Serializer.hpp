#pragma once

#include "GlobalData.hpp"

#include <sstream>

namespace Rain {

enum class SerializerImpl { RAPID_JSON, YAML, BINARY };

void serializeRapidJson(std::ostream &stream, const void *data, TypeInfo &info);
void serializeBinary(std::ostream &stream, const void *data, TypeInfo &info) {}
void serializeYaml(std::ostream &stream, const void *data, TypeInfo &info) {}

void serialize(SerializerImpl serializer, std::ostream &stream, const void *data, TypeInfo &info) {
    switch (serializer) {
        case SerializerImpl::RAPID_JSON: {
            serializeRapidJson(stream, data, info);
        }
        case SerializerImpl::YAML: {
            serializeYaml(stream, data, info);
        }
        case SerializerImpl::BINARY: {
            serializeBinary(stream, data, info);
        }
    }
}

template<typename T>
void serialize(SerializerImpl serializer, std::ostream &stream, const T &value) {
    serialize(serializer, stream, &value, findOrCreateType<T>());
}

} // namespace Rain