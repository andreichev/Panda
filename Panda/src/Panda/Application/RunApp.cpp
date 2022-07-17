#include "RunApp.hpp"
#include "Application.hpp"
#include "ApplicationContext.hpp"

void runPandaApplication(Panda::ApplicationStartupSettings settings) {
    auto context = new Panda::ApplicationContext();
    context->runApplication(settings);
    delete context;
}
