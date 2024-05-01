//
// Created by Admin on 19/03/2022.
//

#include "ExampleLevel.hpp"
#include "RootView.hpp"

#include <Panda/Application/Application.hpp>
#include <PandaUI/PandaUI.hpp>

void ExampleLevel::start(Panda::World *world) {
    using namespace Miren;
    PandaUI::initialize();
    Foundation::Shared<RootView> view = PandaUI::makeView<RootView>();
    PandaUI::Context::shared().setRootView(view);
}
