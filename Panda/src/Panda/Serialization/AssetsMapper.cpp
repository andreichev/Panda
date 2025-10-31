#include "Panda/Serialization/AssetsMapper.hpp"
#include "Panda/GameLogic/GameContext.hpp"
#include "Panda/Base/Base.hpp"

namespace Panda {

void AssetsMapper::toData(MaterialData &data, const MaterialDataDto &dto) {
    data.inputs.clear();
    for (auto &fieldDto : dto.inputs) {
        MaterialField field;
        field.name = fieldDto.name;
        field.type = fieldDto.type;
        field.value = fieldDto.value;
        data.inputs.emplace_back(field);
    }
}

void AssetsMapper::toDto(const MaterialData &data, MaterialDataDto &dto) {
    dto.inputs.clear();
    for (auto &field : data.inputs) {
        MaterialFieldDto fieldDto;
        fieldDto.name = field.name;
        fieldDto.type = field.type;
        fieldDto.value = field.value;
        dto.inputs.emplace_back(fieldDto);
    }
}

} // namespace Panda