#include "Rain/Base.hpp"
#include "Rain/GlobalData.hpp"

#include "Primitives.hpp"

#include <ostream>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

namespace Rain {

using RapidJsonAllocator = RAPIDJSON_DEFAULT_ALLOCATOR;

void rapidJsonSerialize(
    TypeInfo &info, rapidjson::Value &value, const void *data, RapidJsonAllocator &allocator
) {
    if (info.id == PrimitiveTypes::INT) {
        value.SetInt(*(static_cast<const int *>(data)));
    } else if (info.id == PrimitiveTypes::FLOAT) {
        value.SetFloat(*(static_cast<const float *>(data)));
    } else if (info.id == PrimitiveTypes::STRING) {
        const auto *str = static_cast<const std::string *>(data);
        value.SetString(str->c_str(), allocator);
    } else {
        auto &fields = info.fields;
        for (auto &field : fields) {
            if (!field.isRefOrPointer()) {
                rapidjson::Value memberVal(rapidjson::kObjectType);

                auto &memberInfo = findInfo(field.typeId);
                rapidJsonSerialize(memberInfo, memberVal, addOffset(data, field.offset), allocator);
                value.AddMember(
                    rapidjson::GenericStringRef(field.name.c_str()), memberVal, allocator
                );
            }
        }
    }
}

void serializeRapidJson(std::ostream &stream, const void *data, TypeInfo &info) {
    rapidjson::Document document;

    document.SetObject();

    rapidJsonSerialize(info, document, data, document.GetAllocator());

    if (document.HasParseError()) {
        // Parse error
    }

    rapidjson::OStreamWrapper streamWrapper(stream);
    // rapidjson::PrettyWriter prettyWriter(streamWrapper);
    // document.Accept(prettyWriter);
    rapidjson::Writer writer(streamWrapper);
    document.Accept(writer);
}

} // namespace Rain