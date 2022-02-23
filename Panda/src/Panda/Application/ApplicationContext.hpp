//
// Created by Admin on 17.02.2022.
//

#pragma once

#include "Panda/Events/Event.hpp"
#include "Panda/Events/Input.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Renderer/GraphicsContext.hpp"
#include "Panda/Renderer/Renderer.hpp"
#include "Panda/Events/EventQueue.hpp"

namespace Panda {

class Application;

class ApplicationContext {
public:
    static ApplicationContext &get();

    ApplicationContext();
    ~ApplicationContext();
    void setResourcesPath(std::string path);
    std::string &getResourcesPath();
    void pollEvents();
    void postSizeEvent(unsigned int width, unsigned int height);
    void postKeyEvent(Key key, bool down);
    void postMouseEvent(int x, int y);
    Input& getInput();
    Window& getWindow();
    GraphicsContext& getGraphicsContext();
    Renderer& getRenderer();
    Application& getApplication();
    bool isApplicationShouldClose;

private:
    Application* application;
    Input input;
    Window* window;
    Renderer* renderer;
    GraphicsContext* graphicsContext;
    EventQueue eventQueue;
    std::string resourcesPath;
};

} // namespace Panda
