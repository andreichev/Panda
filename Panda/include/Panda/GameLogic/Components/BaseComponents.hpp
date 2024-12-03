//
// Created by Admin on 22.07.2023.
//

#pragma once

#include "Panda/Base/Base.hpp"

#include "Panda/GameLogic/Components/StaticMesh.hpp"
#include "Panda/GameLogic/Components/WorldCamera.hpp"
#include "Panda/GameLogic/Components/DynamicMesh.hpp"
#include "Panda/ScriptEngine/ExternalScript.hpp"
#include "Panda/Assets/Asset.hpp"

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>

namespace Panda {

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
    UUID parent = -1;
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
    Color color;
    Foundation::Shared<Asset> texture;
    UUID textureId = 0;

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent &other) = default;
};

struct StaticMeshComponent final {
    std::vector<StaticMesh> meshes;
};

struct DynamicMeshComponent final {
    std::vector<DynamicMesh> meshes;

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

    void remove(ExternalScript script) {
        scripts.erase(std::remove(scripts.begin(), scripts.end(), script), scripts.end());
    }
};

struct Rigidbody2DComponent final {
    enum class BodyType { STATIC = 0, DYNAMIC, KINEMATIC };
    BodyType type = BodyType::STATIC;
    bool fixedRotation = false;

    // Storage for runtime
    uint8_t runtimeBody[8];
};

struct BoxCollider2DComponent final {
    glm::vec2 offset = {0.0f, 0.0f};
    glm::vec2 size = {0.5f, 0.5f};

    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.0f;

    // Storage for runtime
    uint8_t runtimeFixture[8];
};

} // namespace Panda
