#pragma once

#include "Panda/GameLogic/Entity.hpp"

namespace Panda {

class World;

class Physics2D {
    struct RuntimeBodyData {
        uint8_t data[8];
    };

public:
    void init(World *world);
    bool isInitialized();
    // TODO: Run on separate thread
    void update(World *world, double deltaTime);
    void componentsUpdated(Entity entity);
    void propertiesUpdated(Entity entity);
    /// Applies force to center
    void applyForce(Entity entity, Vec2 force);
    /// Applies impulse to center. impulse = kg * m/s.
    void applyLinearImpulse(Entity entity, Vec2 impulse);
    void setLinearVelocity(Entity entity, Vec2 velocity);
    Vec2 getLinearVelocity(Entity entity);
    float getMass(Entity entity);
    float getFriction(Entity entity);
    void setFriction(Entity entity, float friction);
    void shutdown();

private:
    bool canUse(Entity entity);
    bool isRegistered(Entity entity);
    void registerEntity(Entity entity);
    void removeEntity(Entity entity);

    std::unordered_map<UUID, RuntimeBodyData> m_entityBodyMap;
    std::unordered_map<uint32_t, Entity> m_bodyEntityMap;
    uint32_t m_physicsWorldId;
};

} // namespace Panda