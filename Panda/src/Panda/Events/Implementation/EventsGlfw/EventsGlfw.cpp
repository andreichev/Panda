//
// Created by Admin on 06.02.2022.
//

#include "pndpch.hpp"
#include "EventsGlfw.hpp"
#include "Panda/Application/Application.hpp"

#include <GLFW/glfw3.h>

namespace Panda {

bool EventsGlfw::s_cursorLocked;
bool EventsGlfw::s_cursorStarted;
float EventsGlfw::s_deltaX;
float EventsGlfw::s_deltaY;
float EventsGlfw::s_x;
float EventsGlfw::s_y;
std::vector<WindowSizeListener *> EventsGlfw::s_windowSizeListeners;
std::function<void(GSize)> EventsGlfw::s_frameBufferSizeChangedCallback;

void EventsGlfw::initialize() {
    EventsGlfw::resetCursorPos();
    EventsGlfw::s_cursorLocked = false;
    EventsGlfw::s_x = 0.f;
    EventsGlfw::s_y = 0.f;

    auto windowHandle = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
    glfwSetCursorPosCallback(windowHandle, [](GLFWwindow *windowHandle, double xPos, double yPos) {
        if (EventsGlfw::s_cursorStarted) {
            EventsGlfw::s_deltaX += (float)xPos - EventsGlfw::s_x;
            EventsGlfw::s_deltaY += (float)yPos - EventsGlfw::s_y;
        } else {
            EventsGlfw::s_cursorStarted = true;
        }
        EventsGlfw::s_x = (float)xPos;
        EventsGlfw::s_y = (float)yPos;
    });
}

void EventsGlfw::pollEvents() {
    glfwPollEvents();
}

void EventsGlfw::setFrameBufferSizeCallback(std::function<void(GSize)> callback) {
    s_frameBufferSizeChangedCallback = callback;
    auto windowHandle = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
    glfwSetFramebufferSizeCallback(windowHandle, [](GLFWwindow *window, int width, int height) {
        s_frameBufferSizeChangedCallback({(float)width, (float)height});
    });
}

void EventsGlfw::toggleCursorLock() {
    EventsGlfw::s_cursorLocked = EventsGlfw::s_cursorLocked == false;
    resetCursorPos();
    auto windowHandle = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
    glfwSetInputMode(windowHandle, GLFW_CURSOR, EventsGlfw::s_cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool EventsGlfw::isKeyPressed(Key key) {
    auto windowHandle = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
    auto state = glfwGetKey(windowHandle, static_cast<int>(key));
    return state == GLFW_PRESS;
}

bool EventsGlfw::isKeyJustPressed(Key key) {
    auto windowHandle = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
    auto state = glfwGetKey(windowHandle, static_cast<int>(key));
    return state == GLFW_PRESS;
}

void EventsGlfw::resetCursorPos() {
    EventsGlfw::s_cursorStarted = false;
    EventsGlfw::s_deltaX = 0.f;
    EventsGlfw::s_deltaY = 0.f;
}

bool EventsGlfw::isCursorLocked() {
    return EventsGlfw::s_cursorLocked;
}

void EventsGlfw::addWindowSizeListener(WindowSizeListener *listener) {
    s_windowSizeListeners.push_back(listener);
}

void EventsGlfw::removeWindowSizeListener(WindowSizeListener *listener) {
    s_windowSizeListeners.erase(find(s_windowSizeListeners.begin(), s_windowSizeListeners.end(), listener));
}

} // namespace Panda