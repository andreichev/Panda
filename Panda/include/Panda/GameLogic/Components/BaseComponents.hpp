//
// Created by Admin on 22.07.2023.
//

#pragma once

#include "Panda/Base/Base.hpp"

#include "Panda/GameLogic/Components/StaticMesh.hpp"
#include "Panda/GameLogic/Components/WorldCamera.hpp"
#include "Panda/GameLogic/Components/DynamicMesh.hpp"
#include "Panda/GameLogic/ExternalScript.hpp"
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

// Physics

struct Rigidbody2DComponent {
    enum class BodyType { Static = 0, Dynamic, Kinematic };
    BodyType type = BodyType::Static;
    bool fixedRotation = false;

    // Storage for runtime
    void* runtimeBody = nullptr;

    Rigidbody2DComponent() = default;
    Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
};

struct BoxCollider2DComponent {
    glm::vec2 offset = { 0.0f, 0.0f };
    glm::vec2 size = { 0.5f, 0.5f };

    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.0f;
    float restitutionThreshold = 0.5f;

    // Storage for runtime
    void* runtimeFixture = nullptr;

    BoxCollider2DComponent() = default;
    BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};

} // namespace Panda
