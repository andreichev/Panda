//
// Created by Admin on 19/03/2022.
//

#include "ExampleLevel.hpp"
#include "Panda/Renderer/Renderer2D.hpp"

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

        if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
            Panda::Application::get()->close();
        }
    }

    void onImGuiRender() override {
        // ImGui::ShowDemoWindow();
    }

private:
    float degree = 0.f;
    float colorFactor = 0.f;
};

void ExampleLevel::start(Panda::World *world) {
    using namespace Miren;
    Foundation::Shared<Panda::Entity> entity = world->instantiateEntity();
    Foundation::Shared<ExampleRenderer> dummy = Foundation::makeShared<ExampleRenderer>();
    entity->addComponent(dummy);
}
