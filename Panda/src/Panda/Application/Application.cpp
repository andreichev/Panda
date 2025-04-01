//
// Created by Admin on 22.01.2022.
//

#include "Panda/Application/Application.hpp"
#include "Panda/GameLogic/Input.hpp"
#include "Panda/Random/Random.hpp"

#include <Miren/Miren.hpp>
#include <Fern/Fern.hpp>
#include <chrono>

namespace Panda {

Application *Application::s_instance = nullptr;

Application *Application::get() {
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
    s_instance = nullptr;
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

Application::Application()
    : m_isApplicationShouldClose(false)
    , m_maximumFps(120)
    , m_oneSecondTimeCount(0)
    , m_deltaTimeMillis(0)
    , m_thisSecondFramesCount(0)
    , m_mainWindow(nullptr)
    , m_ImGuiLayer(nullptr) {
    s_instance = this;
    m_timeMillis = getMillis();
    m_layerStack = F_NEW(Foundation::getAllocator(), LayerStack);
    Random::init();
}

void Application::setMainWindow(Fern::Window *window) {
    m_mainWindow = window;
}

Fern::Window *Application::getMainWindow() {
    return m_mainWindow;
}

void Application::createImGuiLayer() {
    PND_ASSERT(m_mainWindow != nullptr, "MAIN WINDOW IS REQUIRED TO CREATE IMGUI LAYER");
    m_ImGuiLayer = F_NEW(Foundation::getAllocator(), ImGuiLayer)(m_mainWindow);
    pushOverlay(m_ImGuiLayer);
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
            // LOG_INFO("FPS: %d", fps);
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
        if (m_ImGuiLayer) {
            m_ImGuiLayer->begin(deltaTime);
            for (Layer *layer : layerStack) {
                layer->onImGuiRender();
            }
            m_ImGuiLayer->end();
        }
        Input::nextFrame();
        Fern::pollEvents();
        processFernEvents();
        Miren::frame();
        // LOG_INFO("APP UPDATE END");
    }
}

ImGuiLayer *Application::getImGuiLayer() {
    return m_ImGuiLayer;
}

void Application::processFernEvents() {
    Fern::EventQueue *eventQueue = Fern::getEventQueue();
    eventQueue->finishWriting();
    Fern::Event *event;
    while ((event = eventQueue->poll()) != nullptr) {
        for (auto it = m_layerStack->rbegin(); it != m_layerStack->rend(); ++it) {
            if (event->isHandled) break;
            (*it)->onEvent(event);
        }
        if (!event->isHandled) { Input::onEvent(event); }
    }
    eventQueue->reset();
}

void Application::close() {
    m_isApplicationShouldClose = true;
}

} // namespace Panda
