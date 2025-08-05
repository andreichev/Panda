#pragma once

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>

namespace Panda {

enum class MaterialFieldType : uint32_t { INTEGER, FLOAT, TEXTURE, UNKNOWN };

struct MaterialField : public Rain::Codable {
    std::string name;
    MaterialFieldType type;
    Foundation::Memory value;
};

struct MaterialData : public Rain::Codable {
    std::vector<MaterialField> inputs;

    MaterialData()
        : inputs() {}

    RAIN_FIELDS_BEGIN(MaterialData)
    RAIN_FIELD(inputs)
    RAIN_FIELDS_END
};

} // namespace Panda