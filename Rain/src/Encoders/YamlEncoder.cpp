#include "Rain/Coders/YamlEncoder.hpp"

#include <yaml-cpp/yaml.h>

namespace Rain {

YamlEncoder::YamlEncoder()
    : out() {}

void YamlEncoder::encode(std::ostream &ostream, void *data, TypeInfo info) {
    YAML::Emitter e;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LocalValueEscapesScope"
    out = &e;
#pragma clang diagnostic pop
    *out << YAML::BeginDoc;
    info.encoderFunc(nullptr, this, info, data);
    *out << YAML::EndDoc;
    ostream << out->c_str();
    out = nullptr;
}

void YamlEncoder::encodeNull(const char *key) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << YAML::Null;
}

void YamlEncoder::beginObject(const char *key) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::BeginMap;
}

void YamlEncoder::encode(const char *key, const bool &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const int &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const uint32_t &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const int64_t &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const uint64_t &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const float &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const double &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const std::string &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const char *&data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::encode(const char *key, const UUID &data) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::Value << data;
}

void YamlEncoder::endObject() {
    *out << YAML::EndMap;
}

void YamlEncoder::beginArray(const char *key) {
    if (key) {
        *out << YAML::Key << key;
    }
    *out << YAML::BeginSeq;
}

void YamlEncoder::endArray() {
    *out << YAML::EndSeq;
}

} // namespace Rain