#pragma once

#import "Fern/Window/Window.hpp"
#import "GraphicsContext/OSX_GLContext.h"

@class NativeWindow;

class CocoaWindow final : public Fern::Window {
public:
    CocoaWindow(
        const char *title,
        Fern::Rect rect,
        Fern::WindowState state,
        Fern::DrawingContextType contextType
    );
    ~CocoaWindow() override;
    void setTitle(const char *title) override;
    Fern::WindowState getState() override;
    void setState(Fern::WindowState state) override;
    void setModifiedState(bool isUnsavedChanges) override;
    bool getModifiedState() override;
    void setResizable(bool isResizable) override;
    void setSize(Fern::Size size) override;
    Fern::Size getSize() override;
    Fern::Size getDpi() override;
    Fern::Vec2 getCursorPosition() override;
    bool setProgressBar(float progress) override;
    bool endProgressBar() override;
    Fern::GraphicsContext *getDrawingContext() override;
    NativeWindow* getNative();
    void didBecomeKey();
    void didResignKey();
private:
    Fern::WindowState m_nextState;
    bool m_isReady;
    bool m_isCursorLocked;
    NativeWindow* m_handle;
    Fern::GraphicsContext *m_graphicsContext;
};
