#include "RunApp.hpp"
#include "Application.hpp"
#include "ApplicationContext.hpp"

void runPandaApplication(Panda::ApplicationStartupSettings settings) {
    Panda::ApplicationContext::get().getApplication().run(settings);
}