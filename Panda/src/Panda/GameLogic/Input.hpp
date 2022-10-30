//
// Created by Admin on 05.02.2022.
//

#pragma once

#include "Panda/Events/Key.hpp"
#include "Panda/Base/Base.hpp"

namespace Panda {

class Window;

class Input {
public:
    // POST EVENTS
    static void setKeyPressed(Key key, bool state);
    static void setMouseButtonPressed(MouseButton mouseButton, bool state);
    static void windowSizeChanged(Size size);
    static void postMouseChangedPosition(int x, int y);
    static void setWindowSize(Size size);
    // GET INPUT DATA
    static bool isKeyPressed(Key key);
    static bool isKeyJustPressed(Key key);
    static bool isMouseButtonPressed(MouseButton mouseButton);
    static bool isMouseButtonJustPressed(MouseButton mouseButton);
    static int getMousePositionX();
    static int getMousePositionY();
    static Size getWindowSize();
    static void update();

private:
    static uint32_t frame;
    // Состояния клавиш
    static bool keys[1024];
    // Номера кадров при нажатии клавиш
    static uint32_t framesKeys[1024];
    // Состояния кнопок мыши
    static bool mouseButtons[8];
    // Номера кадров при нажатии мыши
    static uint32_t framesMouseButtons[4];
    static Size windowSize;
    static int mousePositionX;
    static int mousePositionY;
};

} // namespace Panda
