#pragma once

#include "Panda/Serialization/AssetsDtos.hpp"

namespace Panda {

struct MaterialField {
    std::string name;
    MaterialFieldType type;
    MaterialFieldValue value;

    operator bool() const {
        return !name.empty();
    }
};

struct MaterialData {
    std::vector<MaterialField> inputs;

    void addField(const MaterialField &field);
    MaterialField &getField(const std::string &name);
    std::vector<MaterialField>::iterator findField(const std::string &name);
    void removeField(const MaterialField &field);
    bool hasField(const std::string &name);
};

} // namespace Panda