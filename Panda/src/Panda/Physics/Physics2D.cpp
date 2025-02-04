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
    PND_STATIC_ASSERT(
        sizeof(b2BodyId) <= sizeof(RuntimeBodyData::data),
        "Rigidbody2D runtime body wrong data size"
    );
    PND_STATIC_ASSERT(
        sizeof(b2ShapeId) <= sizeof(BoxCollider2DComponent::runtimeData),
        "Rigidbody2D runtime body wrong data size"
    );
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.restitutionThreshold = 0.5;
    b2WorldId worldId = b2CreateWorld(&worldDef);
    m_physicsWorldId = b2WorldIdToInt(worldId);
    // Register all entities
    auto view = world->m_registry.view<Rigidbody2DComponent>();
    for (auto entityHandle : view) {
        if (!world->isValidEntt(entityHandle)) { continue; }
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
        if (!world->isValidEntt(entityHandle)) { continue; }
        Entity entity = {entityHandle, world};
        if (!isRegistered(entity)) { continue; }
        auto &transform = entity.getTransform();

        RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
        b2BodyId bodyId;
        memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));

        b2Vec2 position = b2Body_GetPosition(bodyId);
        b2Rot rotation = b2Body_GetRotation(bodyId);
        transform.setPosition({position.x, position.y, 0.f});
        float rotationRad = b2Rot_GetAngle(rotation);
        transform.setRotationEuler({0.f, 0.f, glm::degrees(rotationRad)});
        world->convertToLocalSpace(entity);
    }
    //----------------------------------//
    //          CONTACT EVENTS          //
    //----------------------------------//
    b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);
    for (int i = 0; i < contactEvents.endCount; ++i) {
        b2ContactEndTouchEvent event = contactEvents.endEvents[i];
        if (!b2Shape_IsValid(event.shapeIdA) || !b2Shape_IsValid(event.shapeIdB)) { continue; }
        b2BodyId bodyIdA = b2Shape_GetBody(event.shapeIdA);
        b2BodyId bodyIdB = b2Shape_GetBody(event.shapeIdB);
        Entity entityA = m_bodyEntityMap.at(bodyIdA.index1);
        Entity entityB = m_bodyEntityMap.at(bodyIdB.index1);
        ScriptListComponent &scriptListA = entityA.getComponent<ScriptListComponent>();
        for (ExternalScript &script : scriptListA.scripts) {
            script.invokeEndCollisionTouch(entityB.getId());
        }
        ScriptListComponent &scriptListB = entityB.getComponent<ScriptListComponent>();
        for (ExternalScript &script : scriptListB.scripts) {
            script.invokeEndCollisionTouch(entityA.getId());
        }
    }
    for (int i = 0; i < contactEvents.beginCount; ++i) {
        b2ContactBeginTouchEvent event = contactEvents.beginEvents[i];
        if (!b2Shape_IsValid(event.shapeIdA) || !b2Shape_IsValid(event.shapeIdB)) { continue; }
        b2BodyId bodyIdA = b2Shape_GetBody(event.shapeIdA);
        b2BodyId bodyIdB = b2Shape_GetBody(event.shapeIdB);
        Entity entityA = m_bodyEntityMap.at(bodyIdA.index1);
        Entity entityB = m_bodyEntityMap.at(bodyIdB.index1);
        ScriptListComponent &scriptListA = entityA.getComponent<ScriptListComponent>();
        for (ExternalScript &script : scriptListA.scripts) {
            script.invokeBeginCollisionTouch(entityB.getId());
        }
        ScriptListComponent &scriptListB = entityB.getComponent<ScriptListComponent>();
        for (ExternalScript &script : scriptListB.scripts) {
            script.invokeBeginCollisionTouch(entityA.getId());
        }
    }
    //----------------------------------//
    //          SENSOR EVENTS           //
    //----------------------------------//
    b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);
    for (int i = 0; i < sensorEvents.beginCount; ++i) {
        b2SensorBeginTouchEvent event = sensorEvents.beginEvents[i];
        if (!b2Shape_IsValid(event.sensorShapeId) || !b2Shape_IsValid(event.visitorShapeId)) {
            continue;
        }
        b2BodyId bodyIdSensor = b2Shape_GetBody(event.sensorShapeId);
        b2BodyId bodyIdVisitor = b2Shape_GetBody(event.visitorShapeId);
        Entity entitySensor = m_bodyEntityMap.at(bodyIdSensor.index1);
        Entity entityVisitor = m_bodyEntityMap.at(bodyIdVisitor.index1);
        ScriptListComponent &scriptListVisitor = entityVisitor.getComponent<ScriptListComponent>();
        for (ExternalScript &script : scriptListVisitor.scripts) {
            script.invokeBeginSensorOverlap(entitySensor.getId());
        }
    }
    for (int i = 0; i < sensorEvents.endCount; ++i) {
        b2SensorEndTouchEvent event = sensorEvents.endEvents[i];
        if (!b2Shape_IsValid(event.sensorShapeId) || !b2Shape_IsValid(event.visitorShapeId)) {
            continue;
        }
        b2BodyId bodyIdSensor = b2Shape_GetBody(event.sensorShapeId);
        b2BodyId bodyIdVisitor = b2Shape_GetBody(event.visitorShapeId);
        Entity entitySensor = m_bodyEntityMap.at(bodyIdSensor.index1);
        Entity entityVisitor = m_bodyEntityMap.at(bodyIdVisitor.index1);
        ScriptListComponent &scriptListVisitor = entityVisitor.getComponent<ScriptListComponent>();
        for (ExternalScript &script : scriptListVisitor.scripts) {
            script.invokeEndSensorOverlap(entitySensor.getId());
        }
    }
}

void Physics2D::registerEntity(Entity entity) {
    if (!isInitialized()) { return; }
    b2WorldId worldId = IntToB2WorldId(m_physicsWorldId);
    auto transform = entity.calculateWorldSpaceTransform();
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    float rotationDeg = transform.getRotationEuler().z;
    float rotationRad = glm::radians(rotationDeg);
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = rigidbody2DTypeToBox2DBody(rb2d.type);
    bodyDef.position = {transform.getPosition().x, transform.getPosition().y};
    bodyDef.rotation = {cos(rotationRad), sin(rotationRad)};
    bodyDef.fixedRotation = rb2d.fixedRotation;
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
    RuntimeBodyData runtimeData;
    memcpy(&runtimeData.data, &bodyId, sizeof(b2BodyId));
    if (entity.hasComponent<BoxCollider2DComponent>()) {
        auto &bc2d = entity.getComponent<BoxCollider2DComponent>();
        b2Polygon boxShape = b2MakeOffsetBox(
            abs(bc2d.size.x * transform.getScale().x),
            abs(bc2d.size.y * transform.getScale().y),
            {bc2d.offset.x, bc2d.offset.y},
            {1.0f, 0.0f}
        );
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = bc2d.density;
        shapeDef.friction = bc2d.friction;
        shapeDef.restitution = bc2d.restitution;
        shapeDef.isSensor = bc2d.isSensor;
        b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);
        memcpy(&bc2d.runtimeData, &shapeId, sizeof(b2ShapeId));
    }
    m_bodyEntityMap[bodyId.index1] = entity;
    m_entityBodyMap[entity.getId()] = runtimeData;
}

void Physics2D::componentsUpdated(Entity entity) {
    if (!isInitialized()) { return; }
    if (!canUse(entity) && isRegistered(entity)) {
        removeEntity(entity);
        return;
    }
    if (canUse(entity) && !isRegistered(entity)) {
        registerEntity(entity);
        return;
    }
}

void Physics2D::propertiesUpdated(Panda::Entity entity) {
    if (!isInitialized()) { return; }
    if (!isRegistered(entity)) { return; }
    RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
    b2BodyId bodyId;
    memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));

    auto transform = entity.calculateWorldSpaceTransform();
    float rotationDeg = transform.getRotationEuler().z;
    float rotationRad = glm::radians(rotationDeg);
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    b2Body_SetType(bodyId, rigidbody2DTypeToBox2DBody(rb2d.type));
    b2Body_SetTransform(
        bodyId,
        {transform.getPosition().x, transform.getPosition().y},
        {cos(rotationRad), sin(rotationRad)}
    );
    b2Body_SetFixedRotation(bodyId, rb2d.fixedRotation);

    if (entity.hasComponent<BoxCollider2DComponent>()) {
        auto &bc2d = entity.getComponent<BoxCollider2DComponent>();
        b2ShapeId shapeId;
        memcpy(&shapeId, &bc2d.runtimeData, sizeof(b2ShapeId));

        b2Polygon boxShape = b2MakeOffsetBox(
            abs(bc2d.size.x * transform.getScale().x),
            abs(bc2d.size.y * transform.getScale().y),
            {bc2d.offset.x, bc2d.offset.y},
            {1.0f, 0.0f}
        );
        b2Shape_SetPolygon(shapeId, &boxShape);
        b2Shape_SetDensity(shapeId, bc2d.density, true);
        b2Shape_SetFriction(shapeId, bc2d.friction);
        b2Shape_SetRestitution(shapeId, bc2d.restitution);
        PND_ASSERT(
            b2Shape_IsSensor(shapeId) == bc2d.isSensor,
            "CHANGING IS SENSOR IN RUNTIME IS NOT SUPPORTED"
        );
    }
    b2Body_SetLinearVelocity(bodyId, {0, 0});
    b2Body_SetAngularVelocity(bodyId, 0);
}

void Physics2D::removeEntity(Entity entity) {
    if (!isInitialized()) { return; }
    RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
    b2BodyId bodyId;
    memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));
    b2DestroyBody(bodyId);
    m_bodyEntityMap.erase(bodyId.index1);
    m_entityBodyMap.erase(entity.getId());
}

void Physics2D::applyForce(Entity entity, Vec2 force) {
    if (!isInitialized()) { return; }
    RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
    b2BodyId bodyId;
    memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));
    b2Body_ApplyForceToCenter(bodyId, {force.x, force.y}, true);
}

void Physics2D::applyLinearImpulse(Entity entity, Vec2 impulse) {
    if (!isInitialized()) { return; }
    RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
    b2BodyId bodyId;
    memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));
    b2Body_ApplyLinearImpulseToCenter(bodyId, {impulse.x, impulse.y}, true);
}

void Physics2D::setLinearVelocity(Entity entity, Vec2 velocity) {
    if (!isInitialized()) { return; }
    RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
    b2BodyId bodyId;
    memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));
    b2Body_SetLinearVelocity(bodyId, {velocity.x, velocity.y});
}

Vec2 Physics2D::getLinearVelocity(Entity entity) {
    if (!isInitialized()) { return Vec2(); }
    RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
    b2BodyId bodyId;
    memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));
    b2Vec2 result = b2Body_GetLinearVelocity(bodyId);
    return Vec2(result.x, result.y);
}

float Physics2D::getMass(Entity entity) {
    if (!isInitialized()) { return 0; }
    auto &rb2d = entity.getComponent<Rigidbody2DComponent>();
    RuntimeBodyData runtimeData = m_entityBodyMap[entity.getId()];
    b2BodyId bodyId;
    memcpy(&bodyId, &runtimeData.data, sizeof(b2BodyId));
    return b2Body_GetMass(bodyId);
}

float Physics2D::getFriction(Entity entity) {
    if (!isInitialized()) { return 0; }
    auto &bc2d = entity.getComponent<BoxCollider2DComponent>();
    b2ShapeId shapeId;
    memcpy(&shapeId, &bc2d.runtimeData, sizeof(b2ShapeId));
    return b2Shape_GetFriction(shapeId);
}

void Physics2D::setFriction(Entity entity, float friction) {
    if (!isInitialized()) { return; }
    auto &bc2d = entity.getComponent<BoxCollider2DComponent>();
    b2ShapeId shapeId;
    memcpy(&shapeId, &bc2d.runtimeData, sizeof(b2ShapeId));
    b2Shape_SetFriction(shapeId, friction);
}

void Physics2D::shutdown() {
    if (m_physicsWorldId) {
        b2WorldId id = IntToB2WorldId(m_physicsWorldId);
        b2DestroyWorld(id);
        m_physicsWorldId = 0;
    }
    m_bodyEntityMap.clear();
    m_entityBodyMap.clear();
}

bool Physics2D::isRegistered(Entity entity) {
    return m_entityBodyMap.contains(entity.getId());
}

bool Physics2D::canUse(Entity entity) {
    return entity.hasComponent<Rigidbody2DComponent>() &&
           entity.hasComponent<BoxCollider2DComponent>();
}

} // namespace Panda