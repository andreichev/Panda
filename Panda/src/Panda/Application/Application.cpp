//
// Created by Admin on 22.01.2022.
//

#include "pndpch.hpp"
#include "Application.hpp"
#include "ApplicationContext.hpp"

namespace Panda {

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

Application::~Application() {
    delete currentLevel;
    delete world;
}

void Application::run(ApplicationStartupSettings settings) {
    initialize(settings);
    loop();
}

void Application::initialize(ApplicationStartupSettings &settings) {
    Logger::init();
    maximumFps = 60;

    ApplicationContext &context = ApplicationContext::get();
    // Порядок важен
    context.getWindow().initialize(settings.windowTitle, settings.windowSize, settings.isFullScreen);
    context.processEvents();
    GSize windowSize = context.getInput().getWindowSize();
    context.getGraphicsContext().create(windowSize.width, windowSize.height);
    context.getRenderer().initialize();
    world = new World();
    timeMillis = getMillis();
    settings.startupLevel->start(world);
}

void Application::loop() {
    ApplicationContext &context = ApplicationContext::get();
    while (context.isApplicationShouldClose == false) {
        uint64_t lastTime = timeMillis;
        timeMillis = getMillis();
        deltaTimeMillis += timeMillis - lastTime;
        if (deltaTimeMillis < (1000 / maximumFps)) {
            continue;
        }
        oneSecondTimeCount += deltaTimeMillis;

        thisSecondFramesCount++;
        if (oneSecondTimeCount >= 1000) {
            fps = thisSecondFramesCount;
            // PND_INFO("FPS: {}", fps);
            thisSecondFramesCount = 0;
            oneSecondTimeCount -= 1000;
        }

        if (context.getInput().isKeyPressed(Key::ESCAPE)) {
            context.isApplicationShouldClose = true;
        }
        if (context.getInput().isKeyPressed(Key::TAB)) {
            context.getWindow().toggleCursorLock();
        }

        context.getGraphicsContext().beginFrame();
        context.getRenderer().clear();
        world->update(deltaTimeMillis / 1000.0);
        deltaTimeMillis = 0;
        context.getRenderer().checkForErrors();
        context.getGraphicsContext().endFrame();
        context.getWindow().pollEvents();
        context.processEvents();
    }
}

} // namespace Panda
