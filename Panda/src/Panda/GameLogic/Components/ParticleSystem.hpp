#pragma once

#include "Panda/GameLogic/Component.hpp"

namespace Panda {

struct ParticleProps {
    glm::vec2 position;
    glm::vec2 velocity, velocityVariation;
    glm::vec4 colorBegin, colorEnd;
    float sizeBegin, sizeEnd, sizeVariation;
    float lifeTime = 1.0f;
};

class ParticleSystem : public Component {
public:
    ParticleSystem();
    ~ParticleSystem() = default;

    void initialize() override;
    void update(double deltaTime) override;
    void emit(const ParticleProps &particleProps);

private:
    struct Particle {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 colorBegin, colorEnd;
        float rotation = 0.0f;
        float sizeBegin, sizeEnd;

        float lifeTime = 1.0f;
        float lifeRemaining = 0.0f;

        bool active = false;
    };
    std::vector<Particle> m_particlePool;
    uint32_t m_poolIndex = 999;
};

} // namespace Panda