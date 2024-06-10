//
// Created by Admin on 22.07.2023.
//

#pragma once

#include "Panda/Base/Base.hpp"

#include "Panda/GameLogic/Components/StaticMesh.hpp"
#include "Panda/GameLogic/Components/WorldCamera.hpp"
#include "Panda/GameLogic/Components/DynamicMesh.hpp"
#include <Foundation/Foundation.hpp>

namespace Panda {

struct IdComponent final {
    id_t id = 0;
};

struct TagComponent final {
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

struct RelationshipComponent final {
    id_t parentHandle = -1;
    std::vector<id_t> children;

    RelationshipComponent() = default;
    RelationshipComponent(const RelationshipComponent &other) = default;
    RelationshipComponent(id_t parent)
        : parentHandle(parent) {}
};

struct SpriteRendererComponent final {
    Color color;

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent &other) = default;
};

struct StaticMeshComponent final {
    std::vector<StaticMesh> meshes;
};

struct DynamicMeshComponent final {
    std::vector<DynamicMesh> meshes;

    DynamicMeshComponent(const DynamicMeshComponent&) = delete;
    DynamicMeshComponent(DynamicMeshComponent&&) = default;
};

struct CameraComponent final {
    bool isPrimary = true;
    WorldCamera camera;
};

class NativeScript;

struct NativeScriptContainer final {
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

struct NativeScriptListComponent final {
    std::vector<NativeScriptContainer> scripts;

    template<typename T>
    T &add() {
        NativeScriptContainer &container = scripts.emplace_back();
        return container.bind<T>();
    }
};

} // namespace Panda
