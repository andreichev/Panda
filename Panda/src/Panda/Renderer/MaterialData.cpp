#include "Panda/Renderer/MaterialData.hpp"

namespace Panda {

void MaterialData::addField(const MaterialField &field) {
    inputs.emplace_back(field);
}

std::vector<MaterialField>::iterator MaterialData::findField(const std::string &name) {
    return std::find_if(inputs.begin(), inputs.end(), [&](const MaterialField &item) {
        return item.name == name;
    });
}

MaterialField &MaterialData::getField(const std::string &name) {
    return *findField(name);
}

void MaterialData::removeField(const MaterialField &field) {
    auto it = std::remove(inputs.begin(), inputs.end(), field);
    if (it != inputs.end()) { inputs.erase(it); }
}

bool MaterialData::hasField(const std::string &name) {
    return findField(name) != inputs.end();
}

} // namespace Panda