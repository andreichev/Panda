#include "Panda/Application/RunApp.hpp"
#include "Panda/Application/Application.hpp"

#include <Miren/Miren.hpp>

void runPandaApplication(Panda::ApplicationStartupSettings settings) {
    auto application = new Panda::Application(settings);
    application->loop();
    delete application;
}
