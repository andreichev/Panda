//
// Created by Admin on 12.02.2022.
//

#include "ParticlesEmitter.hpp"

#include <Fern/Fern.hpp>

ParticlesEmitter::ParticlesEmitter(
    Panda::TransformComponent *transform, Panda::WorldCamera *camera, Panda::Renderer2D *renderer2D
)
    : m_cameraTransform(transform)
    , m_camera(camera)
    , m_renderer2D(renderer2D)
    , m_material() {}

void ParticlesEmitter::initializeMaterial() {
    Panda::AssetHandler *assetHandler = Panda::GameContext::getAssetHandler();

    Miren::TextureCreate textureCreate =
        Panda::AssetImporterBase::load2DTexture("textures/arbuz1.png");
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = Miren::NEAREST;
    Panda::AssetRef<Panda::TextureAsset> texture =
        assetHandler->createStaticAsset<Panda::TextureAsset>(UUID(), textureCreate);
    ;
    Panda::AssetRef<Panda::ShaderAsset> shader =
        assetHandler->createStaticAsset<Panda::ShaderAsset>(UUID(), "default-shaders/default.frag");
    Panda::MaterialData materialData;
    materialData.inputs = {
        Panda::MaterialField("albedo", Panda::MaterialFieldType::TEXTURE, texture.getId())
    };
    m_material =
        assetHandler->createStaticAsset<Panda::MaterialAsset>(UUID(), materialData, shader);
}

void ParticlesEmitter::update(double deltaTime) {
    degree += 60 * deltaTime;
    colorFactor += deltaTime;

    Panda::Renderer2D::RectData rect1;
    rect1.color = Panda::Color(1.0f, 1.f, 1.f, 1.f);
    rect1.center = Panda::Vec3(0.3f, -0.5f, 0.f);
    rect1.size = Panda::Size(0.4f, 0.4f);
    rect1.rotation = degree;
    rect1.material = m_material;
    m_renderer2D->drawRect(rect1);

    Panda::Renderer2D::RectData rect2;
    rect2.color = Panda::Color(1.0f, abs(sin(colorFactor)), 0.f, 1.f);
    rect2.center = Panda::Vec3(0.6f, 0.6f, 0.f);
    rect2.size = Panda::Size(0.3f, 0.3f);
    m_renderer2D->drawRect(rect2);

    Panda::Renderer2D::RectData rect3;
    rect3.color = Panda::Color(0.f, 0.f, 1.f, 1.f);
    rect3.center = Panda::Vec3(0.1f, 0.8f, 0.f);
    rect3.size = Panda::Size(0.2f, 0.2f);
    m_renderer2D->drawRect(rect3);

    if (Panda::Input::isMouseButtonPressed(Fern::MouseButton::LEFT)) {
        float x = Panda::Input::getMousePositionX();
        float y = Panda::Input::getMousePositionY();
        emit(x, y);
    }

    int touchCount = Panda::Input::touchCount();
    for (int i = 0; i < touchCount; i++) {
        Panda::Input::Touch touch = Panda::Input::getTouch(i);
        emit(touch.x, touch.y);
    }

    m_particleSystem.update(m_renderer2D, deltaTime);
}

void ParticlesEmitter::onImGuiRender() {}

void ParticlesEmitter::emit(float screenX, float screenY) {
    Panda::ParticleProps particleProps;
    particleProps.colorBegin = glm::vec4(1.0f, abs(sin(colorFactor)), abs(cos(colorFactor)), 1.f);
    particleProps.colorEnd = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
    particleProps.sizeBegin = 0.1f;
    particleProps.sizeVariation = 0.1f;
    particleProps.sizeEnd = 1.0f;
    particleProps.lifeTime = 10.0f;
    particleProps.velocity = glm::vec3(0.0f, 0.0f, 0.f);
    particleProps.velocityVariation = glm::vec3(1.5f, 1.5f, 1.5f);
    float cameraZ = m_cameraTransform->getPosition().z;
    glm::mat4 view = glm::inverse(m_cameraTransform->getLocalTransform());
    Panda::Vec3 coord = m_camera->screenCoordToWorld(view, {screenX, screenY}, cameraZ);
    particleProps.position = glm::vec3(coord.x, coord.y, coord.z);
    for (int i = 0; i < 5; i++) {
        m_particleSystem.emit(particleProps);
    }
}
