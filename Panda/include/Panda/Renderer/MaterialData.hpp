#pragma once

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>

namespace Panda {

/* TODO: Add material fields
enum class MaterialFieldType : uint32_t { INTEGER, FLOAT, TEXTURE, UNKNOWN };

struct MaterialField : public Rain::Codable {
    std::string name;
    Foundation::Memory value;
};
*/

struct MaterialData : public Rain::Codable {
    UUID shader;
    std::vector<UUID> textures;

    MaterialData()
        : shader(0)
        , textures() {}

    RAIN_FIELDS_BEGIN(MaterialData)
    RAIN_FIELD(shader)
    RAIN_FIELD(textures)
    RAIN_FIELDS_END
};

} // namespace Panda