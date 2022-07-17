#include <Panda/Renderer/Miren.hpp>

#include <Panda/Thread/Thread.hpp>
#include <Panda/Application/ApplicationContext.hpp>

extern int startApp(int argc, char **argv);

static int32_t renderThread(Panda::Thread *_thread, void *_userData) {
    while (Panda::ApplicationContext::get()->isApplicationShouldClose == false) {
        Panda::Miren::renderFrame();
    }
    Panda::Miren::terminate();
    return 0;
}

int main(int argc, char **argv) {
    Panda::Thread thread;
    thread.init(renderThread, nullptr, 0, "Render thread");

    startApp(argc, argv);

    thread.shutdown();

    return 0;
}