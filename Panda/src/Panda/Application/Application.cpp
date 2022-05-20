//
// Created by Admin on 22.01.2022.
//

#include "pndpch.hpp"
#include "Application.hpp"
#include "ApplicationContext.hpp"
#include "Panda/Renderer/Miren.hpp"

namespace Panda {

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

Application::~Application() {
#ifdef PND_PLATFORM_DESKTOP
    Miren::terminate();
#endif
    delete currentLevel;
    delete world;
}

Application::Application(ApplicationStartupSettings &settings) {
    maximumFps = 60;
    oneSecondTimeCount = 0;
    deltaTimeMillis = 0;
    thisSecondFramesCount = 0;

    ApplicationContext &context = ApplicationContext::get();
    context.processEvents();
    GSize windowSize = context.getInput().getWindowSize();
#ifdef PND_PLATFORM_DESKTOP
    Miren::initialize(windowSize);
#endif
    world = new World();
    timeMillis = getMillis();
    settings.startupLevel->start(world);
    currentLevel = settings.startupLevel;
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
            PND_INFO("FPS: {}", fps);
            thisSecondFramesCount = 0;
            oneSecondTimeCount = 0;
        }

        Miren::beginFrameProcessing();
        world->update(deltaTimeMillis / 1000.0);
        deltaTimeMillis = 0;
        Miren::endFrameProcessing();
#ifdef PND_PLATFORM_DESKTOP
        Miren::renderFrame();
#endif
        context.getWindow().pollEvents();
        context.processEvents();
    }
}

} // namespace Panda
