//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Application/LayerStack.hpp"
#include "Panda/ImGui/ImGuiLayer.hpp"

namespace Panda {

class Application {
public:
    ~Application();
    Application();
    void loop();
    void setMainWindow(Fern::Window *window);
    void createImGuiLayer();
    void pushLayer(Layer *layer);
    void pushOverlay(Layer *layer);
    void processFernEvents();
    ImGuiLayer *getImGuiLayer();
    Fern::Window *getMainWindow();
    void close();
    float getTime();
    static Application *get();

    int fps;

private:
    bool m_isApplicationShouldClose;
    uint64_t m_timeMillis;
    uint64_t m_startTimeMillis;
    // Вспомогательное значение для игры
    float m_gameTime;
    // Таймер до 1 секундны для подсчета FPS (в миллисекундах)
    uint64_t m_oneSecondTimeCount;
    // Время после отрисовки предыдущего кадра
    uint64_t m_deltaTimeMillis;
    int m_thisSecondFramesCount;
    // Ограничение по FPS
    int m_maximumFps;
    LayerStack *m_layerStack;
    ImGuiLayer *m_ImGuiLayer;
    Fern::Window *m_mainWindow;

    static Application *s_instance;
};

} // namespace Panda
