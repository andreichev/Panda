//
// Created by Admin on 17.02.2022.
//

#pragma once

#include "Panda/Events/Event.hpp"
#include "Panda/Events/Input.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Renderer/GraphicsContext.hpp"
#include "Panda/Events/EventQueue.hpp"
#include "Panda/Application/ApplicationStartupSettings.hpp"

namespace Panda {

class Application;

class ApplicationContext {
public:
    static ApplicationContext* get();

    ApplicationContext();
    ~ApplicationContext();
    void runApplication(ApplicationStartupSettings &settings);
    void setResourcesPath(std::string path);
    std::string &getResourcesPath();
    void processEvents();
    void postSizeEvent(unsigned int width, unsigned int height);
    void postKeyEvent(Key key, bool down);
    void postMouseEvent(int x, int y);
    void postMouseButtonEvent(MouseButton button, bool pressed);
    Input &getInput();
    Window &getWindow();
    Application &getApplication();
    bool isApplicationShouldClose;

private:
    static ApplicationContext* s_instance;
    Application *application;
    Input input;
    Window *window;
    EventQueue eventQueue;
    std::string resourcesPath;
};

} // namespace Panda
