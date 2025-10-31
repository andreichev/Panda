#include "Panda/Serialization/ShaderReflectionDataTransformer.hpp"

namespace Panda {

MaterialFieldType toMaterialFieldType(std::string type) {
    if (type == "float") {
        return MaterialFieldType::FLOAT;
    } else if (type == "float4") {
        return MaterialFieldType::VEC4;
    } else {
        return MaterialFieldType::UNKNOWN;
    }
}

MaterialFieldValue getDefaultValueFor(MaterialFieldType type) {
    switch (type) {
        case MaterialFieldType::FLOAT:
            return float(0.f);
        case MaterialFieldType::VEC4:
            return Vec4(0.f, 0.f, 0.f, 0.f);
        case MaterialFieldType::TEXTURE:
            return UUID(0);
        case MaterialFieldType::UNKNOWN:
            break;
    }
    return float(0.f);
}

MaterialData
ShaderReflectionDataTransformer::transformToMaterialData(const ShaderSpirvReflectionData &reflection
) {
    MaterialData data;
    ShaderUniformBufferInfo ubo;
    for (auto item : reflection.ubos) {
        if (item.name == "type.MATERIAL_FIELDS") {
            ubo = item;
            break;
        }
    }
    ShaderTypeInfo typeInfo;
    if (reflection.types.contains(ubo.type)) { typeInfo = reflection.types.at(ubo.type); }
    for (auto &member : typeInfo.members) {
        MaterialFieldType type = toMaterialFieldType(member.type);
        data.inputs.emplace_back(MaterialField(member.name, type, getDefaultValueFor(type)));
    }
    // TODO: Add textures to data
    return data;
}

} // namespace Panda