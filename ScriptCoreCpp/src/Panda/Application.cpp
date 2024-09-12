#include "Panda/Application.hpp"

#include "OuterScriptHook.hpp"

namespace Panda {

uint32_t Application::getWidth() {
    return ExternalCalls::applicationGetWidth();
}

uint32_t Application::getHeight() {
    return ExternalCalls::applicationGetHeight();
}

void Application::quit() {
    ExternalCalls::applicationQuit();
}

} // namespace Panda