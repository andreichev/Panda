#include "Panda/Application.hpp"

#include "OuterScriptHook.hpp"

namespace Panda {

uint32_t Application::getWidth() {
    return ExternalCalls::application_GetWidth();
}

uint32_t Application::getHeight() {
    return ExternalCalls::application_GetHeight();
}

void Application::quit() {
    ExternalCalls::application_Quit();
}

} // namespace Panda