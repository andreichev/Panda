//
// Created by Admin on 22.07.2023.
//

#pragma once

#include "Panda/Base/Base.hpp"

#include "Panda/GameLogic/Components/StaticMesh.hpp"
#include "Panda/GameLogic/Components/DynamicMesh.hpp"
#include <Foundation/Foundation.hpp>

namespace Panda {

struct IdComponent {
    id_t id = 0;
};

struct TagComponent {
    std::string tag;

    TagComponent() = default;
    TagComponent(const TagComponent &other) = default;
    TagComponent(const std::string &tag)
        : tag(tag) {}

    operator std::string &() {
        return tag;
    }
    operator const std::string &() const {
        return tag;
    }
};

struct RelationshipComponent {
    id_t parentHandle = -1;
    std::vector<id_t> children;

    RelationshipComponent() = default;
    RelationshipComponent(const RelationshipComponent &other) = default;
    RelationshipComponent(id_t parent)
        : parentHandle(parent) {}
};

struct StaticMeshComponent {
    std::vector<Foundation::Shared<StaticMesh>> meshes;
};

struct DynamicMeshComponent {
    std::vector<Foundation::Shared<DynamicMesh>> meshes;
};

class NativeScript;

struct NativeScriptContainer {
    Foundation::Shared<NativeScript> instance = nullptr;
    bool initialized = false;
    const char *scriptName;

    template<typename T>
    T &bind() {
        Foundation::Shared<T> script = Foundation::makeShared<T>();
        instance = script;
        scriptName = typeid(T).name();
        return *script;
    }
};

struct NativeScriptListComponent {
    std::vector<NativeScriptContainer> scripts;

    template<typename T>
    T &add() {
        NativeScriptContainer &container = scripts.emplace_back();
        return container.bind<T>();
    }
};

} // namespace Panda