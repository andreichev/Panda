#pragma once

#include "Rain/Coder.hpp"

namespace YAML {
class Emitter;
}

namespace Rain {

class YamlEncoder final : public Encoder {
public:
    YamlEncoder();
    ~YamlEncoder() override = default;

    void encode(std::ostream &, void *data, TypeInfo info) override;
    void encodeNull(const char *key) override;
    void beginObject(const char *key) override;
    void encode(const char *key, const bool &data) override;
    void encode(const char *key, const int &data) override;
    void encode(const char *key, const uint32_t &data) override;
    void encode(const char *key, const int64_t &data) override;
    void encode(const char *key, const uint64_t &data) override;
    void encode(const char *key, const float &data) override;
    void encode(const char *key, const double &data) override;
    void encode(const char *key, const std::string &data) override;
    void encode(const char *key, const char *&data) override;
    void encode(const char *key, const UUID &data) override;
    void endObject() override;
    void beginArray(const char *key) override;
    void endArray() override;

private:
    YAML::Emitter *out;
};

} // namespace Rain
