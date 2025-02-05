//
// Created by Admin on 22.01.2022.
//

#include "pndpch.hpp"

#include "Panda/Application/Application.hpp"
#include "Panda/Application/Initialization/PlatformInit.hpp"
#include "Panda/GameLogic/BasicGameLayer.hpp"
#include "Panda/Events/WindowEvents.hpp"
#include "Panda/Base/Random.hpp"
#include "Panda/GameLogic/Input.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

Application *Application::s_instance = nullptr;

Application *Application::get() {
    PND_ASSERT(s_instance != nullptr, "APP USED BUT NOT INITIALISED");
    return s_instance;
}

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return now.count();
}

Application::~Application() {
    LOG_INFO("APP SHUTDOWN BEGIN");
    F_DELETE(Foundation::getAllocator(), m_layerStack);
#ifdef PLATFORM_DESKTOP
    Miren::terminate();
#endif
    F_DELETE(Foundation::getAllocator(), m_window);
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
    : m_isApplicationShouldClose(false)
    , m_maximumFps(120)
    , m_oneSecondTimeCount(0)
    , m_deltaTimeMillis(0)
    , m_thisSecondFramesCount(0) {
    s_instance = this;
    Foundation::Logger::init();

    m_window = createWindow(settings);
    m_window->setEventQueue(&m_eventQueue);
    m_timeMillis = getMillis();
    m_layerStack = F_NEW(Foundation::getAllocator(), LayerStack);

#ifdef PLATFORM_DESKTOP
    Miren::initialize();
#endif
    Random::init();
    m_ImGuiLayer = F_NEW(Foundation::getAllocator(), ImGuiLayer);
    pushOverlay(m_ImGuiLayer);
}

void Application::startBasicGame(Level *level) {
    pushLayer(F_NEW(Foundation::getAllocator(), BasicGameLayer)(level));
}

void Application::loop() {
    while (!m_isApplicationShouldClose) {
        uint64_t lastTime = m_timeMillis;
        m_timeMillis = getMillis();
        m_deltaTimeMillis += m_timeMillis - lastTime;
        if (m_deltaTimeMillis < (1000 / m_maximumFps)) { continue; }
        m_oneSecondTimeCount += m_deltaTimeMillis;

        m_thisSecondFramesCount++;
        if (m_oneSecondTimeCount >= 1000) {
            fps = m_thisSecondFramesCount;
            // LOG_INFO("FPS: {}", fps);
            m_thisSecondFramesCount = 0;
            m_oneSecondTimeCount = 0;
        }
        double deltaTime = m_deltaTimeMillis / 1000.0;
        if (deltaTime == 0) { deltaTime = 0.00000001; }
        m_deltaTimeMillis = 0;

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
        Input::nextFrame();
        processEvents();
        Miren::frame();
        // LOG_INFO("APP UPDATE END");
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
        }
        for (auto it = m_layerStack->rbegin(); it != m_layerStack->rend(); ++it) {
            if (event->isHandled) break;
            (*it)->onEvent(event);
        }
        if (!event->isHandled) { Input::onEvent(event); }
    }
    m_eventQueue.reset();
}

EventQueue *Application::getEventQueue() {
    return &m_eventQueue;
}

void Application::addWindowSizeObserver(WindowSizeObserver *listener) {
    m_windowSizeListeners.push_back(listener);
}

void Application::windowSizeChanged(Size size) {
    Miren::Rect viewport = Miren::Rect(
        0, 0, size.width * m_window->getDpi().width, size.height * m_window->getDpi().height
    );
    Miren::setViewport(0, viewport);
    for (auto &listener : m_windowSizeListeners) {
        listener->windowSizeChanged(size);
    }
}

void Application::removeWindowSizeObserver(WindowSizeObserver *listener) {
    auto it = std::find(m_windowSizeListeners.begin(), m_windowSizeListeners.end(), listener);
    if (it != m_windowSizeListeners.end()) { m_windowSizeListeners.erase(it); }
}

} // namespace Panda
