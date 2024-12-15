#include "Panda/Physics/Physics2D.hpp"
#include "Panda/GameLogic/World.hpp"

#include <box2d/box2d.h>

namespace Panda {

inline b2BodyType rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType) {
    switch (bodyType) {
        case Rigidbody2DComponent::BodyType::STATIC:
            return b2_staticBody;
        case Rigidbody2DComponent::BodyType::DYNAMIC:
            return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::KINEMATIC:
            return b2_kinematicBody;
    }
    PND_ASSERT(false, "Unknown body type");
    return b2_staticBody;
}

inline Rigidbody2DComponent::BodyType rigidbody2DTypeFromBox2DBody(b2BodyType bodyType) {
    switch (bodyType) {
        case b2_staticBody:
            return Rigidbody2DComponent::BodyType::STATIC;
        case b2_dynamicBody:
            return Rigidbody2DComponent::BodyType::DYNAMIC;
        case b2_kinematicBody:
            return Rigidbody2DComponent::BodyType::KINEMATIC;
        case b2_bodyTypeCount: {
            PND_ASSERT(false, "Unknown body type");
            break;
        }
    }
    PND_ASSERT(false, "Unknown body type");
    return Rigidbody2DComponent::BodyType::STATIC;
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
    // Register all entities
    auto view = world->m_registry.view<Rigidbody2DComponent>();
    for (auto entityHandle : view) {
        if (!world->m_registry.valid(entityHandle)) {
            continue;
        }
        Entity entity = {entityHandle, world};
        registerEntity(entity);
    }
}

bool Physics2D::isInitialized() {
    return m_physicsWorldId;
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
        float rotationRad = b2Rot_GetAngle(rotation);
        transform.setRotationEuler({0.f, 0.f, glm::degrees(rotationRad)});
    }
}

void Physics2D::registerEntity(Entity entity) {
    if (!isInitialized()) {
        return;
    }
    b2WorldId worldId = IntToB2WorldId(m_physicsWorldId);
    auto &transform = entity.getTransform();
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    float rotationDeg = transform.getRotationEuler().z;
    float rotationRad = glm::radians(rotationDeg);
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = rigidbody2DTypeToBox2DBody(rb2d.type);
    bodyDef.position = {transform.getPosition().x, transform.getPosition().y};
    bodyDef.rotation = {cos(rotationRad), sin(rotationRad)};
    bodyDef.fixedRotation = rb2d.fixedRotation;

    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
    memcpy(&rb2d.runtimeBody, &bodyId, sizeof(b2BodyId));

    if (entity.hasComponent<BoxCollider2DComponent>()) {
        auto &bc2d = entity.getComponent<BoxCollider2DComponent>();

        b2Polygon boxShape = b2MakeOffsetBox(
            bc2d.size.x * transform.getScale().x,
            bc2d.size.y * transform.getScale().y,
            {bc2d.offset.x, bc2d.offset.y},
            {1.0f, 0.0f}
        );

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = bc2d.density;
        shapeDef.friction = bc2d.friction;
        shapeDef.restitution = bc2d.restitution;
        b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);
    }
}

void Physics2D::updateEntity(Entity entity) {
    if (!isInitialized()) {
        return;
    }
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    b2BodyId bodyId;
    memcpy(&bodyId, &rb2d.runtimeBody, sizeof(b2BodyId));
    // TODO: Update body properties, not just recreate
    b2DestroyBody(bodyId);
    registerEntity(entity);
}

void Physics2D::removeEntity(Entity entity) {
    if (!isInitialized()) {
        return;
    }
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    b2BodyId bodyId;
    memcpy(&bodyId, &rb2d.runtimeBody, sizeof(b2BodyId));
    b2DestroyBody(bodyId);
}

void Physics2D::applyForce(Entity entity, Vec2 force) {
    if (!isInitialized()) {
        return;
    }
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    b2BodyId bodyId;
    memcpy(&bodyId, &rb2d.runtimeBody, sizeof(b2BodyId));
    b2Body_ApplyForceToCenter(bodyId, {force.x, force.y}, true);
}

void Physics2D::applyLinearImpulse(Entity entity, Vec2 impulse) {
    if (!isInitialized()) {
        return;
    }
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    b2BodyId bodyId;
    memcpy(&bodyId, &rb2d.runtimeBody, sizeof(b2BodyId));
    b2Body_ApplyLinearImpulseToCenter(bodyId, {impulse.x, impulse.y}, true);
}

void Physics2D::setLinearVelocity(Entity entity, Vec2 velocity) {
    if (!isInitialized()) {
        return;
    }
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    b2BodyId bodyId;
    memcpy(&bodyId, &rb2d.runtimeBody, sizeof(b2BodyId));
    b2Body_SetLinearVelocity(bodyId, {velocity.x, velocity.y});
}

Vec2 Physics2D::getLinearVelocity(Entity entity) {
    if (!isInitialized()) {
        return Vec2();
    }
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    b2BodyId bodyId;
    memcpy(&bodyId, &rb2d.runtimeBody, sizeof(b2BodyId));
    b2Vec2 result = b2Body_GetLinearVelocity(bodyId);
    return Vec2(result.x, result.y);
}

void Physics2D::destroy() {
    if (m_physicsWorldId) {
        b2WorldId id = IntToB2WorldId(m_physicsWorldId);
        b2DestroyWorld(id);
        m_physicsWorldId = 0;
    }
}

} // namespace Panda