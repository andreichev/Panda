//
// Created by Admin on 19/03/2022.
//

#include "ExampleLevel.hpp"
#include "Panda/Renderer/Renderer2D.hpp"
#include "Panda/GameLogic/Components/ParticleSystem.hpp"

#include <imgui.h>

class ExampleRenderer : public Panda::Component {
public:
    void initialize() override {}

    void update(double deltaTime) override {
        degree += 60 * deltaTime;
        colorFactor += deltaTime * 0.3f;
        if (colorFactor >= 1.f) {
            colorFactor = 0.f;
        }
        Panda::RectData rect1;
        rect1.color = Panda::Color(1.0f, 0.5f, colorFactor, 1.f);
        rect1.origin = Panda::Point(30, 30);
        rect1.size = Panda::Size(40, 20);
        rect1.rotation = degree;
        Panda::Renderer2D::drawRect(rect1);

        Panda::RectData rect2;
        rect2.color = Panda::Color(1.0f, 0.f, 0.f, 1.f);
        rect2.origin = Panda::Point(60, 60);
        rect2.size = Panda::Size(10, 10);
        Panda::Renderer2D::drawRect(rect2);

        Panda::RectData rect3;
        rect3.color = Panda::Color(0.f, 0.f, 1.f, 1.f);
        rect3.origin = Panda::Point(10, 80);
        rect3.size = Panda::Size(10, 10);
        Panda::Renderer2D::drawRect(rect3);

        if (Panda::Input::isMouseButtonPressed(Panda::MouseButton::LEFT)) {
            for (int i = 0; i < 2; i++) {
                Panda::ParticleProps particleProps;

                particleProps.colorBegin = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};
                particleProps.colorEnd = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
                particleProps.sizeBegin = 30.f, particleProps.sizeVariation = 0.3f;
                particleProps.sizeEnd = 0.0f;
                particleProps.lifeTime = 1.0f;
                particleProps.velocity = {0.0f, 0.0f};
                particleProps.velocityVariation = {200.0f, 200.0f};
                particleProps.position = {
                    Panda::Input::getMousePositionX(), Panda::Input::getMousePositionY()};

                m_particleSystem->emit(particleProps);
            }
        }

        if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
            Panda::Application::get()->close();
        }
    }

    void onImGuiRender() override {
        // ImGui::ShowDemoWindow();
    }

    void setParticleSysyem(Foundation::Shared<Panda::ParticleSystem> particleSystem) {
        m_particleSystem = particleSystem;
    }

private:
    Foundation::Shared<Panda::ParticleSystem> m_particleSystem;
    float degree = 0.f;
    float colorFactor = 0.f;
};

void ExampleLevel::start(Panda::World *world) {
    using namespace Miren;
    Foundation::Shared<Panda::Entity> entity = world->instantiateEntity();
    Foundation::Shared<Panda::ParticleSystem> particle =
        Foundation::makeShared<Panda::ParticleSystem>();
    Foundation::Shared<ExampleRenderer> dummy = Foundation::makeShared<ExampleRenderer>();
    entity->addComponent(dummy);
    entity->addComponent(particle);
    dummy->setParticleSysyem(particle);
}
