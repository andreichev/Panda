//
// Created by Admin on 05.02.2022.
//

#pragma once

#include "Panda/Base/Base.hpp"

#include <Fern/Events/Event.hpp>
#include <Fern/Events/Key.hpp>
#include <Fern/Base/Base.hpp>

namespace Panda {

class Input {
public:
    struct Touch {
        int id;
        float x;
        float y;

        Touch(int id, float x, float y)
            : id(id)
            , x(x)
            , y(y) {}
    };

    // GET INPUT DATA
    static bool isKeyPressed(Fern::Key key);
    static bool isKeyJustPressed(Fern::Key key);
    static bool isMouseButtonPressed(Fern::MouseButton mouseButton);
    static bool isMouseButtonJustPressed(Fern::MouseButton mouseButton);
    static int touchCount();
    static Touch getTouch(int index);
    static double getMousePositionX();
    static double getMousePositionY();
    static double getMouseDeltaX();
    static double getMouseDeltaY();
    static double getMouseViewportPositionX();
    static double getMouseViewportPositionY();
    static double getMouseScrollX();
    static double getMouseScrollY();
    static bool isTrackpadScroll();
    static Size getWindowSize();
    static void onEvent(Fern::Event *event);
    static void setViewportFrame(Rect frame);
    static void nextFrame();

private:
    // POST EVENTS
    static void postTouchBeganEvent(int id, float x, float y);
    static void postTouchMovedEvent(int id, float x, float y);
    static void postTouchEndedEvent(int id);
    static void setKeyPressed(Fern::Key key, bool state);
    static void setMouseButtonPressed(Fern::MouseButton mouseButton, bool state);
    static void postMouseChangedPosition(double x, double y, double dx, double dy);
    static void postScrollEvent(double x, double y, bool isTrackpad);
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
    static Rect viewportFrame;
    static double mousePositionX;
    static double mousePositionY;
    static double mouseDeltaX;
    static double mouseDeltaY;
    static double mouseScrollX;
    static double mouseScrollY;
    static bool _isTrackpadScroll;
    static std::vector<Touch> activeTouches;
};

} // namespace Panda
