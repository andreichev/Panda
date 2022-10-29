//
// Created by Admin on 22.01.2022.
//

#include "Application.hpp"
#include "Panda/Application/Initialization/PlatformInit.hpp"
#include "Panda/GameLogic/BasicGameLayer.hpp"
#include "Panda/Events/WindowEvents.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

Application *Application::s_instance = nullptr;

Application *Application::get() {
    PND_ASSERT(s_instance != nullptr, "APP USED BUT NOT INITIALISED");
    return s_instance;
}

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

Application::~Application() {
    delete m_window;
}

void Application::pushLayer(Layer *layer) {
    m_layerStack.pushLayer(layer);
    layer->onAttach();
}

void Application::pushOverlay(Layer *layer) {
    m_layerStack.pushOverlay(layer);
    layer->onAttach();
}

Application::Application(ApplicationStartupSettings &settings)
    : isApplicationShouldClose(false)
    , maximumFps(60)
    , oneSecondTimeCount(0)
    , deltaTimeMillis(0)
    , thisSecondFramesCount(0) {
    s_instance = this;
    Logger::init();

    m_window = createWindow(settings);
    m_window->setEventQueue(&m_eventQueue);
    timeMillis = getMillis();

#ifdef PND_PLATFORM_DESKTOP
    Panda::Miren::initialize();
#endif

    startBasicGame(settings.startupLevel);

    m_ImGuiLayer = new ImGuiLayer();
    pushOverlay(m_ImGuiLayer);
}

void Application::startBasicGame(Level *level) {
    pushLayer(new BasicGameLayer(level));
}

void Application::loop() {
    while (isApplicationShouldClose == false) {
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
        double deltaTime = deltaTimeMillis / 1000.0;
        deltaTimeMillis = 0;

        Miren::renderSemaphoreWait();
        for (Layer *layer : m_layerStack) {
            layer->onUpdate(deltaTime);
        }
        m_ImGuiLayer->begin(deltaTime);
        for (Layer *layer : m_layerStack) {
            layer->onImGuiRender();
        }
        m_ImGuiLayer->end();
        m_window->pollEvents();
        processEvents();
        Miren::renderSemaphorePost();
    }
}

Window *Application::getWindow() {
    return m_window;
}

void Application::processEvents() {
    Event *event;
    while ((event = m_eventQueue.poll()) != nullptr) {
        if (event->type == EventType::WindowResize) {
            const WindowResizeEvent *ev = dynamic_cast<const WindowResizeEvent *>(event);
            windowSizeChanged(UISize(ev->getWidth(), ev->getHeight()));
        } else if (event->type == EventType::WindowClose) {
            close();
        }
        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
            if (event->isHandled)
                break;
            (*it)->onEvent(event);
        }
        m_eventQueue.release(event);
    }
}

EventQueue *Application::getEventQueue() {
    return &m_eventQueue;
}

void Application::addWindowSizeListener(WindowSizeListener *listener) {
    m_windowSizeListeners.push_back(listener);
}

void Application::windowSizeChanged(UISize size) {
    for (auto &listener : m_windowSizeListeners) {
        listener->windowSizeChanged(size);
    }
}

void Application::removeWindowSizeListener(WindowSizeListener *listener) {
    auto it = std::find(m_windowSizeListeners.begin(), m_windowSizeListeners.end(), listener);
    if (it != m_windowSizeListeners.end()) {
        m_windowSizeListeners.erase(it);
    }
}

} // namespace Panda
