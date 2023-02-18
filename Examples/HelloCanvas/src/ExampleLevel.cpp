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
        Panda::RectData rect;
        rect.color = Panda::Color(1.0f, 0.f, 0.0f, 1.f);
        rect.origin = Panda::Point(20, 20);
        rect.size = Panda::Size(60, 20);
        Panda::Renderer2D::drawRect(rect);
        if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
            Panda::Application::get()->close();
        }
    }

    void onImGuiRender() override {
        ImGui::ShowDemoWindow();
    }
};

void ExampleLevel::start(Panda::World *world) {
    using namespace Miren;

    Foundation::Shared<ExampleRenderer> dummy = Foundation::makeShared<ExampleRenderer>();

    Foundation::Shared<Panda::Entity> entity = world->instantiateEntity();
    entity->addComponent(dummy);
}
