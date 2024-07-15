//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Application/ApplicationStartupSettings.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Application/LayerStack.hpp"
#include "Panda/Events/EventQueue.hpp"
#include "Panda/Events/WindowSizeObserver.hpp"
#include "Panda/ImGui/ImGuiLayer.hpp"
#include "Panda/GameLogic/Level.hpp"

namespace Panda {

class Application {
public:
    ~Application();
    Application(ApplicationStartupSettings &settings);
    void loop();

    void startBasicGame(Level *level);
    void pushLayer(Layer *layer);
    void pushOverlay(Layer *layer);
    ImGuiLayer *getImGuiLayer() {
        return m_ImGuiLayer;
    }
    void processEvents();
    Window *getWindow();
    void addWindowSizeObserver(WindowSizeObserver *listener);
    void removeWindowSizeObserver(WindowSizeObserver *listener);
    inline void close() {
        m_isApplicationShouldClose = true;
    }
    inline bool isRunning() {
        return !m_isApplicationShouldClose;
    }
    int fps;
    EventQueue *getEventQueue();

    static Application *get();

private:
    void windowSizeChanged(Size size);

    bool m_isApplicationShouldClose;
    uint64_t m_timeMillis;
    // Таймер до 1 секундны для подсчета FPS (в миллисекундах)
    uint64_t m_oneSecondTimeCount;
    // Время после отрисовки предыдущего кадра
    uint64_t m_deltaTimeMillis;
    int m_thisSecondFramesCount;
    // Ограничение по FPS
    int m_maximumFps;
    LayerStack *m_layerStack;
    EventQueue m_eventQueue;
    Window *m_window;
    std::vector<WindowSizeObserver *> m_windowSizeListeners;
    ImGuiLayer *m_ImGuiLayer;

    static Application *s_instance;
};

} // namespace Panda
