//
// Created by Admin on 17.02.2022.
//

#pragma once

#include "Panda/Window/Window.hpp"

namespace Panda {

class PandaWindowIOSImpl : public Window {
public:
    PandaWindowIOSImpl();
    ~PandaWindowIOSImpl() override = default;
    bool isFullScreen() override {
        return true;
    }
    void setFullScreen(bool isFullScreen) override {}
    void setTitle(const char *title) override {}
    void pollEvents() override {}
    void toggleCursorLock() override {}
    bool isCursorLocked() override {
        return true;
    }
    void setCursor(Cursor cursor) override {}
    void setEventQueue(EventQueue *eventQueue) override;
    Size getSize() override {
        return m_size;
    }
    void setSize(Size size) override {}
    Size getDpi() override {
        return m_dpi;
    }
    void setResizable(bool isResizable) override {}
    bool isMaximized() override {}
    void setMaximized(bool isMaximized) override {}
    const char *getClipboardText() override {}
    void setClipboardText(const char *text) override {}

private:
    Size m_size;
    Size m_dpi;
};

} // namespace Panda
