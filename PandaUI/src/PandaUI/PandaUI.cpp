//
// Created by Michael Andreichev on 11.11.2023.
//

#include "PandaUI/PandaUI.hpp"

#include <Panda/Application/Application.hpp>

namespace PandaUI {

void initialize() {
    Panda::Application *app = Panda::Application::get();
    PND_ASSERT(app != nullptr, "APP IS NOT INITIALIZED BEFORE PandaUI");
    app->pushOverlay(NEW(Foundation::getAllocator(), PandaAppUILayer));
}

} // namespace PandaUI