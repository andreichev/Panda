//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/Application/ApplicationStartupSettings.hpp"
#include "Panda/GameLogic/Level.hpp"
#include "Panda/GameLogic/World.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Renderer/Renderer.hpp"
#include "Panda/Events/Input.hpp"

namespace Panda {

class Application {
public:
    ~Application();

    int fps;
    void run(ApplicationStartupSettings settings);

private:
    void initialize(ApplicationStartupSettings &settings);
    void loop();

    World *world;
    Level *currentLevel;
    uint64_t timeMillis;
    // Таймер до 1 секудны для подсчета FPS (в миллисекундах)
    uint64_t oneSecondTimeCount;
    // Время после отрисовки предыдущего кадра
    uint64_t deltaTimeMillis;
    int thisSecondFramesCount;
    // Ограничение по FPS
    int maximumFps;
};

} // namespace Panda
