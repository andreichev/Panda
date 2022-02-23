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
    void windowSizeChanged(GSize size);
    void postMouseChangedPosition(int x, int y);
    // GET INPUT DATA
    bool isKeyPressed(Key key);
    void addWindowSizeListener(WindowSizeListener *listener);
    int getMousePositionX();
    int getMousePositionY();
    void removeWindowSizeListener(WindowSizeListener *listener);
private:
    bool keys[1024];
    std::vector<WindowSizeListener *> windowSizeListeners;
    int mousePositionX;
    int mousePositionY;

};

} // namespace Panda
