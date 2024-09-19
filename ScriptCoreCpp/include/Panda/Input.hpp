//
// Created by Michael Andreichev on 19.09.2024.
//

#pragma once

#include "Key.hpp"

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

    static bool isKeyPressed(Key key);
    static bool isKeyJustPressed(Key key);
    static bool isMouseButtonPressed(MouseButton mouseButton);
    static bool isMouseButtonJustPressed(MouseButton mouseButton);
    static int touchCount();
    static Touch getTouch(int index);
    static double getMousePositionX();
    static double getMousePositionY();
    static double getMouseScrollX();
    static double getMouseScrollY();
};

} // namespace Panda
