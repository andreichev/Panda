//
// Created by Admin on 06.02.2022.
//

#pragma once

#include "Panda/Events/IEvents.hpp"

namespace Panda {

class EventsGlfw : public IEvents {
    void initialize() override;
    void pollEvents() override;
    void setFrameBufferSizeCallback(std::function<void(GSize)>) override;
    void addWindowSizeListener(WindowSizeListener *listener) override;
    void removeWindowSizeListener(WindowSizeListener *listener) override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    bool isKeyPressed(Key key) override;
    bool isKeyJustPressed(Key key) override;
    float getDeltaX() override;
    float getDeltaY() override;

private:
    static void resetCursorPos();
    static std::vector<WindowSizeListener *> s_windowSizeListeners;
    static std::function<void(GSize)> s_frameBufferSizeChangedCallback;
    static bool s_cursorLocked;
    static bool s_cursorStarted;
    static float s_deltaX;
    static float s_deltaY;
    static float s_x;
    static float s_y;
};

} // namespace Panda