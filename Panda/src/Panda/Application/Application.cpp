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
    LOG_INFO("APP SHUTDOWN BEGIN");
    Miren::renderSemaphoreWait();
    DELETE(Foundation::getAllocator(), m_layerStack);
#ifdef PLATFORM_DESKTOP
    Miren::terminate();
#endif
    DELETE(Foundation::getAllocator(), m_window);
    LOG_INFO("APP SHUTDOWN END");
}

void Application::pushLayer(Layer *layer) {
    m_layerStack->pushLayer(layer);
    layer->onAttach();
}

void Application::pushOverlay(Layer *layer) {
    m_layerStack->pushOverlay(layer);
    layer->onAttach();
}

Application::Application(ApplicationStartupSettings &settings)
    : isApplicationShouldClose(false)
    , maximumFps(60)
    , oneSecondTimeCount(0)
    , deltaTimeMillis(0)
    , thisSecondFramesCount(0) {
    s_instance = this;
    Foundation::Logger::init();

    m_window = createWindow(settings);
    m_window->setEventQueue(&m_eventQueue);
    timeMillis = getMillis();
    m_layerStack = NEW(Foundation::getAllocator(), LayerStack);

#ifdef PLATFORM_DESKTOP
    Miren::initialize();
#endif
    Miren::renderSemaphoreWait();
    startBasicGame(settings.startupLevel);
    m_ImGuiLayer = NEW(Foundation::getAllocator(), ImGuiLayer);
    pushOverlay(m_ImGuiLayer);
    Miren::renderSemaphorePost();
}

void Application::startBasicGame(Level *level) {
    pushLayer(NEW(Foundation::getAllocator(), BasicGameLayer)(level));
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
            LOG_INFO("FPS: {}", fps);
            thisSecondFramesCount = 0;
            oneSecondTimeCount = 0;
        }
        double deltaTime = deltaTimeMillis / 1000.0;
        deltaTimeMillis = 0;

        Miren::renderSemaphoreWait();
        // LOG_INFO("APP UPDATE BEGIN");
        LayerStack &layerStack = *m_layerStack;
        for (Layer *layer : layerStack) {
            layer->onUpdate(deltaTime);
        }
        m_ImGuiLayer->begin(deltaTime);
        for (Layer *layer : layerStack) {
            layer->onImGuiRender();
        }
        m_ImGuiLayer->end();
        m_window->pollEvents();
        processEvents();
        Miren::frame();
        // LOG_INFO("APP UPDATE END");
        Miren::renderSemaphorePost();
    }
}

Window *Application::getWindow() {
    return m_window;
}

void Application::processEvents() {
    m_eventQueue.finishWriting();
    Event *event;
    while ((event = m_eventQueue.poll()) != nullptr) {
        if (event->type == EventType::WindowResize) {
            const WindowResizeEvent *ev = static_cast<const WindowResizeEvent *>(event);
            windowSizeChanged(Size(ev->getWidth(), ev->getHeight()));
        } else if (event->type == EventType::WindowClose) {
            close();
        }
        for (auto it = m_layerStack->rbegin(); it != m_layerStack->rend(); ++it) {
            if (event->isHandled)
                break;
            (*it)->onEvent(event);
        }
    }
    m_eventQueue.reset();
}

EventQueue *Application::getEventQueue() {
    return &m_eventQueue;
}

void Application::addWindowSizeListener(WindowSizeListener *listener) {
    m_windowSizeListeners.push_back(listener);
}

void Application::windowSizeChanged(Size size) {
    Miren::setViewport(Miren::Rect(0, 0, size.width, size.height));
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
