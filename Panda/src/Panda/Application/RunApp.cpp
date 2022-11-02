#include "RunApp.hpp"
#include "Panda/Application/Application.hpp"

#include <Miren/Miren.hpp>
#include <Foundation/Thread.hpp>

void *operator new(size_t size) {
    // std::cout << "NEW" << std::endl;
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    // std::cout << "DELETE" << std::endl;
    free(ptr);
}

void runPandaApplication(Panda::ApplicationStartupSettings settings) {
    auto application = new Panda::Application(settings);
    application->loop();
    delete application;
}
