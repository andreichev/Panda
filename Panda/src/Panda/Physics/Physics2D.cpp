#include "Physics2D.hpp"
#include "Panda/GameLogic/World.hpp"

#include <box2d/box2d.h>

namespace Panda {

inline b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType) {
    switch (bodyType) {
        case Rigidbody2DComponent::BodyType::Static:
            return b2_staticBody;
        case Rigidbody2DComponent::BodyType::Dynamic:
            return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Kinematic:
            return b2_kinematicBody;
    }

    PND_ASSERT(false, "Unknown body type");
    return b2_staticBody;
}

inline Rigidbody2DComponent::BodyType Rigidbody2DTypeFromBox2DBody(b2BodyType bodyType) {
    switch (bodyType) {
        case b2_staticBody:
            return Rigidbody2DComponent::BodyType::Static;
        case b2_dynamicBody:
            return Rigidbody2DComponent::BodyType::Dynamic;
        case b2_kinematicBody:
            return Rigidbody2DComponent::BodyType::Kinematic;
        case b2_bodyTypeCount: {
            PND_ASSERT(false, "Unknown body type");
            break;
        }
    }

    PND_ASSERT(false, "Unknown body type");
    return Rigidbody2DComponent::BodyType::Static;
}

uint32_t b2WorldIdToInt(b2WorldId id) {
    return ((uint32_t)id.index1 << 16) | (uint32_t)id.revision;
}

b2WorldId IntToB2WorldId(uint32_t val) {
    b2WorldId id;
    id.index1 = (uint16_t)(val >> 16);
    id.revision = (uint16_t)(val & 0xFFFF);
    return id;
}

void Physics2D::init(World *world) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.restitutionThreshold = 0.5;
    b2WorldId worldId = b2CreateWorld(&worldDef);
    m_physicsWorldId = b2WorldIdToInt(worldId);

    auto view = world->m_registry.view<Rigidbody2DComponent>();
    for (auto entityHandle : view) {
        if (!world->m_registry.valid(entityHandle)) {
            continue;
        }
        Entity entity = {entityHandle, world};
        auto &transform = entity.getTransform();
        auto &rb2d = entity.getComponent<Rigidbody2DComponent>();

        float rotation = transform.getRotationEuler().z;
        b2BodyDef bodyDef;
        bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.type);
        bodyDef.position = {transform.getPosition().x, transform.getPosition().y};
        bodyDef.rotation = {cos(rotation), sin(rotation)};
        bodyDef.fixedRotation = rb2d.fixedRotation;

        b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
        memcpy(&rb2d.runtimeBody, &bodyId, sizeof(b2BodyId));

        if (entity.hasComponent<BoxCollider2DComponent>()) {
            auto &bc2d = entity.getComponent<BoxCollider2DComponent>();

            b2Polygon boxShape = b2MakeOffsetBox(
                bc2d.size.x * transform.getScale().x,
                bc2d.size.y * transform.getScale().y,
                b2Vec2(bc2d.offset.x, bc2d.offset.y),
                {1.0f, 0.0f}
            );

            b2ShapeDef shapeDef = b2DefaultShapeDef();
            shapeDef.density = bc2d.density;
            shapeDef.friction = bc2d.friction;
            shapeDef.restitution = bc2d.restitution;
            b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);
        }
    }
}

void Physics2D::update(World *world, double deltaTime) {
    const int32_t subStepCount = 4;
    const int32_t positionIterations = 2;
    b2WorldId worldId = IntToB2WorldId(m_physicsWorldId);
    b2World_Step(worldId, deltaTime, subStepCount);

    // Retrieve transform from Box2D
    auto view = world->m_registry.view<Rigidbody2DComponent>();
    for (auto entityHandle : view) {
        if (!world->m_registry.valid(entityHandle)) {
            continue;
        }
        Entity entity = {entityHandle, world};
        auto &transform = entity.getTransform();
        auto &rb2d = entity.getComponent<Rigidbody2DComponent>();

        b2BodyId bodyId;
        memcpy(&bodyId, &rb2d.runtimeBody, sizeof(b2BodyId));

        b2Vec2 position = b2Body_GetPosition(bodyId);
        b2Rot rotation = b2Body_GetRotation(bodyId);
        transform.setPosition({position.x, position.y, 0.f});
        transform.setRotationEuler({0.f, 0.f, acos(rotation.c)});
    }
}

void Physics2D::destroy() {
    if (m_physicsWorldId) {
        b2WorldId id = IntToB2WorldId(m_physicsWorldId);
        b2DestroyWorld(id);
    }
}

} // namespace Panda