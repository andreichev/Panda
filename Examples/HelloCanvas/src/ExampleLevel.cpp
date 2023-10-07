//
// Created by Admin on 19/03/2022.
//

#include "ExampleLevel.hpp"
#include "OrthographicCameraMove.hpp"

#include <Panda/Application/Application.hpp>
#include <Panda/Renderer/Renderer2D.hpp>
#include <Panda/GameLogic/Components/ParticleSystem.hpp>
#include <Panda/GameLogic/Components/Camera.hpp>

class CameraSizeObserver : public Panda::NativeScript, Panda::WindowSizeObserver {
public:
    void initialize() override {
        Panda::Application::get()->addWindowSizeObserver(this);
    }

    ~CameraSizeObserver() {
        Panda::Application::get()->removeWindowSizeObserver(this);
    }

    void update(double deltaTime) override {}

    void windowSizeChanged(Panda::Size size) override {
        m_camera->viewportSizeChanged(size);
    }

    void setCamera(Panda::Camera *camera) {
        m_camera = camera;
    }

private:
    Panda::Camera *m_camera;
};

class ExampleRenderer : public Panda::NativeScript {
public:
    void initialize() override {
        m_texture = Foundation::makeShared<Panda::Texture>("textures/arbuz1.png");
        m_camera->viewportSizeChanged(Panda::Application::get()->getWindow()->getSize());
    }

    ~ExampleRenderer() {}

    void update(double deltaTime) override {
        degree += 60 * deltaTime;
        colorFactor += deltaTime;

        Panda::Renderer2D::RectData rect1;
        rect1.color = Panda::Color(1.0f, 1.f, 1.f, 1.f);
        rect1.origin = Panda::Vec3(0.3f, -0.5f, 0.f);
        rect1.size = Panda::Size(0.4f, 0.4f);
        rect1.rotation = degree;
        rect1.texture = m_texture;
        Panda::Renderer2D::drawRect(rect1);

        Panda::Renderer2D::RectData rect2;
        rect2.color = Panda::Color(1.0f, abs(sin(colorFactor)), 0.f, 1.f);
        rect2.origin = Panda::Vec3(0.6f, 0.6f, 0.f);
        rect2.size = Panda::Size(0.3f, 0.3f);
        Panda::Renderer2D::drawRect(rect2);

        Panda::Renderer2D::RectData rect3;
        rect3.color = Panda::Color(0.f, 0.f, 1.f, 1.f);
        rect3.origin = Panda::Vec3(0.1f, 0.8f, 0.f);
        rect3.size = Panda::Size(0.2f, 0.2f);
        Panda::Renderer2D::drawRect(rect3);

        if (Panda::Input::isMouseButtonPressed(Panda::MouseButton::LEFT)) {
            Panda::ParticleProps particleProps;
            particleProps.colorBegin =
                glm::vec4(1.0f, abs(sin(colorFactor)), abs(cos(colorFactor)), 1.f);
            particleProps.colorEnd = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            particleProps.sizeBegin = 0.1f;
            particleProps.sizeVariation = 0.1f;
            particleProps.sizeEnd = 0.0f;
            particleProps.lifeTime = 20.0f;
            particleProps.velocity = glm::vec3(0.0f, 0.0f, 0.f);
            particleProps.velocityVariation = glm::vec3(1.5f, 1.5f, 1.5f);
            float x = Panda::Input::getMousePositionX();
            float y = Panda::Input::getMousePositionY();
            // float cameraZ = m_camera->getEntity().getTransform().getPosition().z;
            // LOG_INFO("CAMERA HEIGHT: {}", cameraZ);
            Panda::Vec3 coord = m_camera->screenCoordToWorld({x, y}, 20.f);
            particleProps.position = glm::vec3(coord.x, coord.y, coord.z);
            for (int i = 0; i < 5; i++) {
                m_particleSystem->emit(particleProps);
            }
        }

        if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
            Panda::Application::get()->close();
        }
    }

    void onImGuiRender() override {}

    void setParticleSystem(Panda::ParticleSystem *particleSystem) {
        m_particleSystem = particleSystem;
    }

    void setCamera(Panda::Camera *camera) {
        m_camera = camera;
    }

private:
    Panda::ParticleSystem *m_particleSystem;
    Panda::Camera *m_camera;
    Foundation::Shared<Panda::Texture> m_texture;
    float degree = 0.f;
    float colorFactor = 0.f;
};

void ExampleLevel::start(Panda::World *world) {
    using namespace Miren;
    Panda::Entity entity = world->instantiateEntity();
    Panda::Camera &camera = entity.addNativeScript<Panda::Camera>();
    Panda::Renderer2D::setCamera(&camera);
    entity.getTransform().translate(0.f, 0.f, 1.f);
    Panda::ParticleSystem &particle = entity.addNativeScript<Panda::ParticleSystem>();
    ExampleRenderer &dummy = entity.addNativeScript<ExampleRenderer>();
    OrthographicCameraMove &cameraMove = entity.addNativeScript<OrthographicCameraMove>();
    CameraSizeObserver &cameraSizeObserver = entity.addNativeScript<CameraSizeObserver>();
    cameraMove.setCamera(&camera);
    cameraSizeObserver.setCamera(&camera);
    dummy.setParticleSystem(&particle);
    dummy.setCamera(&camera);
}
