#pragma once

#include <Rain/Rain.hpp>

namespace Panda {

struct ShaderTypeMemberInfo : public Rain::Codable {
    std::string name;
    std::string type;
    int offset;

    RAIN_FIELDS_BEGIN(ShaderTypeMemberInfo)
    RAIN_FIELD(name)
    RAIN_FIELD(type)
    RAIN_FIELD(offset)
    RAIN_FIELDS_END
};

struct ShaderTypeInfo : public Rain::Codable {
    std::string name;
    std::vector<ShaderTypeMemberInfo> members;

    RAIN_FIELDS_BEGIN(ShaderTypeInfo)
    RAIN_FIELD(name)
    RAIN_FIELD(members)
    RAIN_FIELDS_END
};

struct ShaderUniformBufferInfo : public Rain::Codable {
    std::string type;
    std::string name;
    int block_size;
    int set;
    int binding;

    RAIN_FIELDS_BEGIN(ShaderUniformBufferInfo)
    RAIN_FIELD(type)
    RAIN_FIELD(name)
    RAIN_FIELD(block_size)
    RAIN_FIELD(set)
    RAIN_FIELD(binding)
    RAIN_FIELDS_END
};

struct ShaderImageInfo : public Rain::Codable {
    std::string type;
    std::string name;
    int set;
    int binding;

    RAIN_FIELDS_BEGIN(ShaderImageInfo)
    RAIN_FIELD(type)
    RAIN_FIELD(name)
    RAIN_FIELD(set)
    RAIN_FIELD(binding)
    RAIN_FIELDS_END
};

struct ShaderSamplerInfo : public Rain::Codable {
    std::string type;
    std::string name;
    int set;
    int binding;

    RAIN_FIELDS_BEGIN(ShaderSamplerInfo)
    RAIN_FIELD(type)
    RAIN_FIELD(name)
    RAIN_FIELD(set)
    RAIN_FIELD(binding)
    RAIN_FIELDS_END
};

struct ShaderSpirvReflectionData : public Rain::Codable {
    std::unordered_map<std::string, ShaderTypeInfo> types;
    std::vector<ShaderImageInfo> separate_images;
    std::vector<ShaderSamplerInfo> separate_samplers;
    std::vector<ShaderUniformBufferInfo> ubos;

    RAIN_FIELDS_BEGIN(ShaderSpirvReflectionData)
    RAIN_FIELD(types)
    RAIN_FIELD(separate_images)
    RAIN_FIELD(separate_samplers)
    RAIN_FIELD(ubos)
    RAIN_FIELDS_END
};

} // namespace Panda