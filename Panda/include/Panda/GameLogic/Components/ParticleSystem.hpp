#pragma once

#include "Panda/GameLogic/NativeScript.hpp"

namespace Panda {

#define MAX_PARTICLES 10000

struct ParticleProps {
    glm::vec3 position;
    glm::vec3 velocity, velocityVariation;
    glm::vec4 colorBegin, colorEnd;
    float sizeBegin, sizeEnd, sizeVariation;
    float lifeTime = 1.0f;
};

class ParticleSystem : public NativeScript {
public:
    ParticleSystem();
    ~ParticleSystem() = default;

    void initialize() override;
    void update(double deltaTime) override;
    void emit(const ParticleProps &particleProps);

private:
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 colorBegin, colorEnd;
        float rotation = 0.0f;
        float sizeBegin, sizeEnd;

        float lifeTime = 1.0f;
        float lifeRemaining = 0.0f;

        bool active = false;
    };
    Particle m_particlePool[MAX_PARTICLES];
    uint32_t m_poolIndex = MAX_PARTICLES - 1;
};

} // namespace Panda
