//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/Application/ApplicationStartupSettings.hpp"
#include "Panda/GameLogic/ILevel.hpp"
#include "Panda/GameLogic/World.hpp"
#include "Panda/Window/IWindow.hpp"
#include "Panda/Renderer/IRenderer.hpp"
#include "Panda/Events/IEvents.hpp"

namespace Panda {

class Application {
public:
    int fps;

    static Application &get();

    IWindow &getWindow();
    IEvents &getEvents();
    IRenderer &getRenderer();
    void run(Panda::Unique<ApplicationStartupSettings> settings);

private:
    void initialize(Panda::Unique<ApplicationStartupSettings> &settings);
    void loop();

    Shared<IWindow> window;
    Shared<IRenderer> renderer;
    Shared<World> world;
    Shared<ILevel> currentLevel;
    Shared<IEvents> events;
    double time;
    // Таймер до 1 секудны для подсчета FPS
    float oneSecondTimeCount;
    int thisSecondFramesCount;
    // Ограничение по FPS
    int maximumFps;
    // Время после отрисовки предыдущего кадра
    float deltaTime;
};

} // namespace Panda