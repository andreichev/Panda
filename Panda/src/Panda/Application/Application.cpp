//
// Created by Admin on 22.01.2022.
//

#include "pndpch.hpp"
#include "Application.hpp"
#include "Panda/Platform/PlatformInit.hpp"
#include "Panda/Platform/RendererInit.hpp"

namespace Panda {

Application &Application::get() {
    static Application application;
    return application;
}

void Application::run(Panda::Unique<ApplicationStartupSettings> settings) {
    initialize(settings);
    loop();
}

void Application::initialize(Panda::Unique<ApplicationStartupSettings> &settings) {
    Logger::init();
    maximumFps = 60;
    window = Shared<IWindow>(createWindow());
    events = Shared<IEvents>(createEvents());
    renderer = Shared<IRenderer>(RendererInit::getRenderer());
    // Порядок важен
    window->initialize(settings->windowTitle, settings->windowSize, settings->isFullScreen);
    events->initialize();
    renderer->initialize();
    world = createShared<World>();
    events->setFrameBufferSizeCallback([this](GSize size) { renderer->setViewportSize(size); });
    time = window->getTime();
    settings->startupLevel->start(world.get());
}

void Application::loop() {
    while (window->isShouldClose() == false) {
        double lastTime = time;
        time = window->getTime();
        deltaTime += (float)time - (float)lastTime;
        if (deltaTime - (1.f / (float)maximumFps) < 0) {
            continue;
        }
        oneSecondTimeCount += deltaTime;

        thisSecondFramesCount++;
        if (oneSecondTimeCount > 1.0) {
            fps = thisSecondFramesCount;
            PND_INFO("FPS: {}", fps);
            thisSecondFramesCount = 0;
            oneSecondTimeCount -= 1.f;
        }

        renderer->clear();
        if (events->isKeyJustPressed(Key::ESCAPE)) {
            window->setShouldClose(true);
        }
        if (events->isKeyJustPressed(Key::TAB)) {
            events->toggleCursorLock();
        }

        world->update(deltaTime);
        deltaTime = 0.f;
        // renderer->checkForErrors();
        window->swapBuffers();
        events->pollEvents();
    }
}

IWindow &Application::getWindow() {
    return *window;
}

IEvents &Application::getEvents() {
    return *events;
}

IRenderer &Application::getRenderer() {
    return *renderer;
}

} // namespace Panda