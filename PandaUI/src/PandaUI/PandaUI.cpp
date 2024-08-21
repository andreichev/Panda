//
// Created by Michael Andreichev on 11.11.2023.
//

#include "PandaUI/PandaUI.hpp"
#include "PandaUI/ApplicationLayer/Layer.hpp"

#include <Panda/Application/Application.hpp>

namespace PandaUI {

void initialize() {
    Panda::Application *app = Panda::Application::get();
    PND_ASSERT(app != nullptr, "APP IS NOT INITIALIZED BEFORE PandaUI");
    PND_ASSERT(
        !Context::isInitialized(), "UICONTEXT ALREADY INITIALIZED. IT CAN BE INITIALIZED ONLY ONCE"
    );
    app->pushOverlay(F_NEW(Foundation::getAllocator(), Layer));
}

} // namespace PandaUI