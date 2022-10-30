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

#ifdef PND_PLATFORM_DESKTOP
static int32_t renderThread(Foundation::Thread *_thread, void *_userData) {
    while (Panda::Application::get()->isRunning()) {
        Miren::renderFrame();
    }
    Miren::terminate();
    return 0;
}
#endif

void runPandaApplication(Panda::ApplicationStartupSettings settings) {
#ifdef PND_PLATFORM_DESKTOP
    Foundation::Thread thread;
    thread.init(renderThread, nullptr, 0, "Render thread");
#endif

    auto application = new Panda::Application(settings);
    application->loop();
    delete application;

#ifdef PND_PLATFORM_DESKTOP
    thread.shutdown();
#endif
}
