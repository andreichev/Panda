//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "pndpch.hpp"

#include "Panda/Application/ApplicationStartupSettings.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Application/LayerStack.hpp"
#include "Panda/Events/EventQueue.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Panda/ImGui/ImGuiLayer.hpp"

namespace Panda {

class Application {
public:
    ~Application();
    Application(ApplicationStartupSettings &settings);
    void loop();

    void startBasicGame(Level *level);
    void pushLayer(Layer *layer);
    void pushOverlay(Layer *layer);
    void processEvents();
    Window *getWindow();
    void addWindowSizeListener(WindowSizeListener *listener);
    void removeWindowSizeListener(WindowSizeListener *listener);
    inline void close() {
        isApplicationShouldClose = true;
    }
    inline bool isRunning() {
        return isApplicationShouldClose == false;
    }
    int fps;
    EventQueue *getEventQueue();

    static Application *get();

private:
    void windowSizeChanged(UISize size);

    bool isApplicationShouldClose;
    uint64_t timeMillis;
    // Таймер до 1 секудны для подсчета FPS (в миллисекундах)
    uint64_t oneSecondTimeCount;
    // Время после отрисовки предыдущего кадра
    uint64_t deltaTimeMillis;
    int thisSecondFramesCount;
    // Ограничение по FPS
    int maximumFps;
    LayerStack m_layerStack;
    EventQueue m_eventQueue;
    Window *m_window;
    std::vector<WindowSizeListener *> m_windowSizeListeners;
    ImGuiLayer *m_ImGuiLayer;

    static Application *s_instance;
};

} // namespace Panda
