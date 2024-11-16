#pragma once

namespace Panda {

class World;

class Physics2D {
public:
    void init(World *world);
    void update(World *world, double deltaTime);
    void destroy();

private:
    uint32_t m_physicsWorldId;
};

} // namespace Panda