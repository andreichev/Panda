#include "ParticleSystem.hpp"

#include "Panda/Renderer/Renderer2D.hpp"
#include "Panda/Base/Random.hpp"

#include <glm/gtx/compatibility.hpp>

namespace Panda {

ParticleSystem::ParticleSystem() {
    m_particlePool.resize(1000);
}

void ParticleSystem::initialize() {}

void ParticleSystem::update(double deltaTime) {
    for (Particle &particle : m_particlePool) {
        if (!particle.active)
            continue;

        if (particle.lifeRemaining <= 0.0f) {
            particle.active = false;
            continue;
        }

        particle.lifeRemaining -= deltaTime;
        particle.position += particle.velocity * (float)deltaTime;
        particle.rotation += 0.01f * (float)deltaTime;

        RectData rect;
        rect.origin = particle.position;
        float life = particle.lifeRemaining / particle.lifeTime;
        glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
        rect.color = color;
        float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
        rect.size = Size(size, size);
        rect.rotation = particle.rotation;
        Renderer2D::drawRect(rect);
    }
}

void ParticleSystem::emit(const ParticleProps &particleProps) {
    Particle &particle = m_particlePool[m_poolIndex];
    particle.active = true;
    particle.position = particleProps.position;
    particle.rotation = Random::getFloat() * 360.f;

    // Velocity
    particle.velocity = particleProps.velocity;
    particle.velocity.x += particleProps.velocityVariation.x * (Random::getFloat() - 0.5f);
    particle.velocity.y += particleProps.velocityVariation.y * (Random::getFloat() - 0.5f);

    // Color
    particle.colorBegin = particleProps.colorBegin;
    particle.colorEnd = particleProps.colorEnd;

    particle.lifeTime = particleProps.lifeTime;
    particle.lifeRemaining = particleProps.lifeTime;
    particle.sizeBegin =
        particleProps.sizeBegin + particleProps.sizeVariation * (Random::getFloat() - 0.5f);
    particle.sizeEnd = particleProps.sizeEnd;

    m_poolIndex = --m_poolIndex % m_particlePool.size();
}

} // namespace Panda