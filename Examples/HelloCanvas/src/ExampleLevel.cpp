//
// Created by Admin on 19/03/2022.
//

#include "ExampleLevel.hpp"
#include "Panda/Renderer/Renderer2D.hpp"
#include "Panda/GameLogic/Components/ParticleSystem.hpp"
#include "Panda/GameLogic/Components/OrthographicCamera.hpp"
#include "OrthographicCameraMove.hpp"

#include <imgui.h>

class ExampleRenderer : public Panda::Component {
public:
    void initialize() override {
        m_texture = Foundation::makeShared<Panda::Texture>("textures/arbuz1.png");
    }

    void update(double deltaTime) override {
        degree += 60 * deltaTime;
        colorFactor += deltaTime * 0.3f;
        if (colorFactor >= 1.f) {
            colorFactor = 0.f;
        }
        Panda::Renderer2D::RectData rect1;
        rect1.color = Panda::Color(1.0f, 0.5f, colorFactor, 1.f);
        rect1.origin = Panda::Point(0.3f, -0.5f);
        rect1.size = Panda::Size(0.4f, 0.2f);
        rect1.rotation = degree;
        rect1.texture = m_texture;
        Panda::Renderer2D::drawRect(rect1);

        Panda::Renderer2D::RectData rect2;
        rect2.color = Panda::Color(1.0f, 0.f, 0.f, 1.f);
        rect2.origin = Panda::Point(0.6f, 0.6f);
        rect2.size = Panda::Size(0.1f, 0.1f);
        Panda::Renderer2D::drawRect(rect2);

        Panda::Renderer2D::RectData rect3;
        rect3.color = Panda::Color(0.f, 0.f, 1.f, 1.f);
        rect3.origin = Panda::Point(0.1f, 0.8f);
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
            Panda::Point coord = m_camera->screenCoordToWorld(Panda::Point(x, y));
            particleProps.position = glm::vec2(coord.x, coord.y);
            for (int i = 0; i < 5; i++) {
                m_particleSystem->emit(particleProps);
            }
        }

        if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
            Panda::Application::get()->close();
        }
    }

    void onImGuiRender() override {
        ImGui::Begin("Renderer2D statistics");
        ImGui::Text("FPS: %d", Panda::Application::get()->fps);
        auto stats = Panda::Renderer2D::getStats();
        ImGui::Text("Quads count: %d", stats.quadCount);
        ImGui::Text("Vertices count: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices count: %d", stats.getTotalIndexCount());
        ImGui::Text("Draw calls: %d", stats.drawCalls);
        ImGui::End();
    }

    void setParticleSysyem(Foundation::Shared<Panda::ParticleSystem> particleSystem) {
        m_particleSystem = particleSystem;
    }

    void setCamera(Foundation::Shared<Panda::OrthographicCamera> camera) {
        m_camera = camera;
    }

private:
    Foundation::Shared<Panda::ParticleSystem> m_particleSystem;
    Foundation::Shared<Panda::OrthographicCamera> m_camera;
    Foundation::Shared<Panda::Texture> m_texture;
    float degree = 0.f;
    float colorFactor = 0.f;
};

void ExampleLevel::start(Panda::World *world) {
    using namespace Miren;
    Foundation::Shared<Panda::OrthographicCamera> camera =
        Foundation::makeShared<Panda::OrthographicCamera>();
    world->setOrthographicCamera(camera);
    Foundation::Shared<Panda::Entity> entity = world->instantiateEntity();
    Foundation::Shared<Panda::ParticleSystem> particle =
        Foundation::makeShared<Panda::ParticleSystem>();
    Foundation::Shared<ExampleRenderer> dummy = Foundation::makeShared<ExampleRenderer>();
    Foundation::Shared<OrthographicCameraMove> cameraMove =
        Foundation::makeShared<OrthographicCameraMove>();
    entity->addComponent(dummy);
    entity->addComponent(particle);
    entity->addComponent(camera);
    entity->addComponent(cameraMove);
    cameraMove->setCamera(camera);
    dummy->setParticleSysyem(particle);
    dummy->setCamera(camera);
}
