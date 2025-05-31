//
// Created by Admin on 22.07.2023.
//

#pragma once

#include "Panda/Base/Base.hpp"

#include "Panda/Assets/StaticMeshAsset.hpp"
#include "Panda/Assets/DynamicMeshAsset.hpp"
#include "Panda/GameLogic/Components/WorldCamera.hpp"
#include "Panda/ScriptEngine/ExternalScript.hpp"

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>

namespace Panda {

#ifdef PND_EDITOR
struct EditorMetadataComponent final {
    bool isDeleted = false;
};
#endif

struct IdComponent final : public Rain::Codable {
    UUID id;

    IdComponent() = default;
    IdComponent(const IdComponent &other) = default;
    IdComponent(const UUID &id)
        : id(id) {}

    RAIN_FIELDS_BEGIN(IdComponent)
    RAIN_FIELD(id)
    RAIN_FIELDS_END
};

struct TagComponent final : public Rain::Codable {
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

    RAIN_FIELDS_BEGIN(TagComponent)
    RAIN_FIELD(tag)
    RAIN_FIELDS_END
};

struct RelationshipComponent final : public Rain::Codable {
    UUID parent = 0;
    std::vector<UUID> children;

    RelationshipComponent() = default;
    RelationshipComponent(const RelationshipComponent &other) = default;
    RelationshipComponent(UUID parent)
        : parent(parent) {}

    RAIN_FIELDS_BEGIN(RelationshipComponent)
    RAIN_FIELD(parent)
    RAIN_FIELD(children)
    RAIN_FIELDS_END
};

struct SpriteRendererComponent final {
    // Component
    Color color = Color(0.f, 0.f, 0.f, 1.f);
    int cols = 1;
    int rows = 1;
    int index = 0;
    UUID textureId = 0;

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent &other) = default;

    // Cache
    Foundation::Shared<Asset> asset;
    void resetCache() {
        asset = nullptr;
    }
};

struct StaticMeshComponent final {
    std::vector<StaticMeshAsset> meshes;
};

struct DynamicMeshComponent final {
    std::vector<DynamicMeshAsset> meshes;

    DynamicMeshComponent() = default;
    DynamicMeshComponent(const DynamicMeshComponent &other) = default;
    DynamicMeshComponent &operator=(DynamicMeshComponent &other) = default;
};

struct CameraComponent final {
    bool isPrimary = true;
    WorldCamera camera;
};

struct ScriptListComponent final {
    std::vector<ExternalScript> scripts;

    void add(ExternalScript script) {
        scripts.emplace_back(script);
    }

    void releaseFields() {
        for (ExternalScript &script : scripts) {
            for (ScriptField &field : script.getFields()) {
                field.value.release();
            }
        }
    }

    void remove(ExternalScript script) {
        scripts.erase(std::remove(scripts.begin(), scripts.end(), script), scripts.end());
    }
};

struct Rigidbody2DComponent final {
    enum class BodyType : uint32_t { STATIC = 0, DYNAMIC, KINEMATIC };
    BodyType type = BodyType::STATIC;
    bool fixedRotation = false;
};

struct BoxCollider2DComponent final {
    glm::vec2 offset = {0.0f, 0.0f};
    glm::vec2 size = {0.5f, 0.5f};
    bool isSensor = false;

    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.0f;

    // Storage for runtime
    uint8_t runtimeData[8];
};

} // namespace Panda
