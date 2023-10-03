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

    Panda::Vec3 screenCoordToWorld(
        Panda::Vec3 cameraPosition, Panda::Vec2 viewportSize, Panda::Vec2 screen) {
        // Вычисляем половину ширины и высоты экрана
        double halfScreenWidth = viewportSize.width / 2.0;
        double halfScreenHeight = viewportSize.height / 2.0;

        // Переводим координаты экрана в нормализованные координаты (от -1 до 1)
        double xNormalized = (screen.x - halfScreenWidth) / halfScreenWidth;
        double yNormalized = (screen.y - halfScreenHeight) / halfScreenHeight;

        // Вычисляем смещение между позицией камеры и точкой на экране
        double offsetX = cameraPosition.z * xNormalized;
        double offsetY = -cameraPosition.z * yNormalized;

        // Координаты мира
        return Panda::Vec3(cameraPosition.x + offsetX, cameraPosition.y + offsetY, 0.f);
    }

    void update(double deltaTime) override {
        degree += 60 * deltaTime;
        colorFactor += deltaTime * 0.3f;
        if (colorFactor >= 1.f) {
            colorFactor = 0.f;
        }

        Panda::Renderer2D::RectData rect1;
        rect1.color = Panda::Color(1.0f, 0.5f, colorFactor, 1.f);
        rect1.origin = Panda::Vec3(0.3f, -0.5f, 0.f);
        rect1.size = Panda::Size(0.4f, 0.2f);
        rect1.rotation = degree;
        rect1.texture = m_texture;
        Panda::Renderer2D::drawRect(rect1);

        Panda::Renderer2D::RectData rect2;
        rect2.color = Panda::Color(1.0f, 0.f, 0.f, 1.f);
        rect2.origin = Panda::Vec3(0.6f, 0.6f, 0.f);
        rect2.size = Panda::Size(0.2f, 0.2f);
        Panda::Renderer2D::drawRect(rect2);

        Panda::Renderer2D::RectData rect3;
        rect3.color = Panda::Color(0.f, 0.f, 1.f, 1.f);
        rect3.origin = Panda::Vec3(0.1f, 0.8f, 0.f);
        rect3.size = Panda::Size(0.1f, 0.1f);
        Panda::Renderer2D::drawRect(rect3);

        if (Panda::Input::isMouseButtonPressed(Panda::MouseButton::LEFT)) {
            Panda::ParticleProps particleProps;
            particleProps.colorBegin = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f);
            particleProps.colorEnd = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            particleProps.sizeBegin = 0.1f;
            particleProps.sizeVariation = 0.1f;
            particleProps.sizeEnd = 0.0f;
            particleProps.lifeTime = 2.0f;
            particleProps.velocity = glm::vec2(0.0f, 0.0f);
            particleProps.velocityVariation = glm::vec2(0.7f, 0.7f);
            float x = Panda::Input::getMousePositionX();
            float y = Panda::Input::getMousePositionY();
            Panda::Transform &cameraTransform = m_camera->getEntity().getTransform();
            Panda::Vec3 cameraPosition = {cameraTransform.getPosition().x,
                cameraTransform.getPosition().y,
                cameraTransform.getPosition().z};
            Panda::Size viewportSize = m_camera->getViewportSize();
            Panda::Vec3 coord = screenCoordToWorld(cameraPosition, viewportSize, {x, y});
            particleProps.position = glm::vec2(coord.x, coord.y);
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
    Panda::ParticleSystem &particle = entity.addNativeScript<Panda::ParticleSystem>();
    ExampleRenderer &dummy = entity.addNativeScript<ExampleRenderer>();
    OrthographicCameraMove &cameraMove = entity.addNativeScript<OrthographicCameraMove>();
    CameraSizeObserver &cameraSizeObserver = entity.addNativeScript<CameraSizeObserver>();
    cameraMove.setCamera(&camera);
    cameraSizeObserver.setCamera(&camera);
    dummy.setParticleSystem(&particle);
    dummy.setCamera(&camera);
}
