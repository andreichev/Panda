#pragma once

#include "Panda/Window/Window.hpp"

#ifndef __OBJC__
typedef void *id;
#endif

class CocoaWindow final : public Panda::Window {
public:
    CocoaWindow(const char *title, Panda::Size size, bool isFullscreen, bool isMaximized);
    ~CocoaWindow() override;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    void setTitle(const char *title) override;
    bool isMaximized() override;
    void setMaximized(bool isMaximized) override;
    void setResizable(bool isResizable) override;
    void setSize(Panda::Size size) override;
    void pollEvents() override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void setCursor(Panda::Cursor cursor) override;
    void setEventQueue(Panda::EventQueue *eventQueue) override;
    Panda::Size getSize() override;
    Panda::Size getDpi() override;
    const char *getClipboardText() override;
    void setClipboardText(const char *text) override;

    Panda::Vec2 getCursorPos();
    id getNative() {
        return m_handle;
    }

private:
    bool m_isCursorLocked;
    Panda::EventQueue *m_eventQueue;
    id m_handle;
};
