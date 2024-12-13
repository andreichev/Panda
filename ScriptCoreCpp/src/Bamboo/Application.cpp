#include "Bamboo/Application.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

uint32_t Application::getWidth() {
    return Panda::ExternalCalls::application_GetWidth();
}

uint32_t Application::getHeight() {
    return Panda::ExternalCalls::application_GetHeight();
}

void Application::loadWorld(const char *name) {
    Panda::ExternalCalls::world_Load(name);
}

void Application::quit() {
    Panda::ExternalCalls::application_Quit();
}

} // namespace Bamboo