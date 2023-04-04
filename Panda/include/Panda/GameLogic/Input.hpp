//
// Created by Admin on 05.02.2022.
//

#pragma once

#include "Panda/Events/Event.hpp"
#include "Panda/Events/Key.hpp"
#include "Panda/Base/Base.hpp"

namespace Panda {

class Window;

class Input {
public:
    // GET INPUT DATA
    static bool isKeyPressed(Key key);
    static bool isKeyJustPressed(Key key);
    static bool isMouseButtonPressed(MouseButton mouseButton);
    static bool isMouseButtonJustPressed(MouseButton mouseButton);
    static double getMousePositionX();
    static double getMousePositionY();
    static double getMouseScrollX();
    static double getMouseScrollY();
    static Size getWindowSize();
    static void onEvent(Event *event);
    static void nextFrame();

private:
    // POST EVENTS
    static void setKeyPressed(Key key, bool state);
    static void setMouseButtonPressed(MouseButton mouseButton, bool state);
    static void windowSizeChanged(Size size);
    static void postMouseChangedPosition(double x, double y);
    static void postScrollEvent(double x, double y);
    static void setWindowSize(Size size);

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
    static double mousePositionX;
    static double mousePositionY;
    static double mouseScrollX;
    static double mouseScrollY;
};

} // namespace Panda
