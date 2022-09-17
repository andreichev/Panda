#include "RunApp.hpp"
#include "Panda/Thread/Thread.hpp"
#include "Panda/Application/Application.hpp"
#include "Panda/Renderer/Miren.hpp"

void *operator new(size_t size) {
    // std::cout << "NEW" << std::endl;
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    // std::cout << "DELETE" << std::endl;
    free(ptr);
}

#ifdef PND_PLATFORM_DESKTOP
static int32_t renderThread(Panda::Thread *_thread, void *_userData) {
    Panda::Miren::initialize();
    while (Panda::Application::get()->isRunning()) {
        Panda::Miren::renderFrame();
    }
    Panda::Miren::terminate();
    return 0;
}
#endif

void runPandaApplication(Panda::ApplicationStartupSettings settings) {
    auto application = new Panda::Application(settings);

#ifdef PND_PLATFORM_DESKTOP
    Panda::Thread thread;
    thread.init(renderThread, nullptr, 0, "Render thread");
#endif

    application->loop();
    delete application;

#ifdef PND_PLATFORM_DESKTOP
    thread.shutdown();
#endif
}
