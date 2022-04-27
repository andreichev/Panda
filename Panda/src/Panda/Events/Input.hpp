//
// Created by Admin on 05.02.2022.
//

#pragma once

#include "Key.hpp"
#include "WindowSizeListener.hpp"

namespace Panda {

class Window;

class Input {
public:
    // POST EVENTS
    void setKeyPressed(Key key, bool state);
    void setMouseButtonPressed(MouseButton mouseButton, bool state);
    void windowSizeChanged(GSize size);
    void postMouseChangedPosition(int x, int y);
    // GET INPUT DATA
    bool isKeyPressed(Key key);
    bool isKeyJustPressed(Key key);
    bool isMouseButtonPressed(MouseButton mouseButton);
    bool isMouseButtonJustPressed(MouseButton mouseButton);
    void addWindowSizeListener(WindowSizeListener *listener);
    int getMousePositionX();
    int getMousePositionY();
    void removeWindowSizeListener(WindowSizeListener *listener);
    GSize getWindowSize();
    void eventProcessingFinished();

private:
    uint32_t frame;
    // Состояния клавиш
    bool keys[1024];
    // Номера кадров при нажатии клавиш
    uint32_t framesKeys[1024];
    // Состояния кнопок мыши
    bool mouseButtons[8];
    // Номера кадров при нажатии мыши
    uint32_t framesMouseButtons[4];
    GSize windowSize;
    std::vector<WindowSizeListener *> windowSizeListeners;
    int mousePositionX;
    int mousePositionY;
};

} // namespace Panda
