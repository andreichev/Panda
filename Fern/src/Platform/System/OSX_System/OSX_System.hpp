#pragma once

#include "Fern/Platform/PlatformSystem.hpp"

namespace Fern {

class OSX_System final : public PlatformSystem {
public:
    OSX_System();
    Window *createWindow(
        const char *title, Rect rect, WindowState state, DrawingContextType contextType
    ) override;
    void disposeWindow(Window *window) override;
    void pollEvents() override;
    Vec2 getCursorPosition() const override;
    const char *getClipboardText() override;
    void setClipboardText(const char *text) override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void setCursor(Cursor cursor) override;
    uint64_t getMilliSeconds() const override;
    const std::filesystem::path &getResourcesPath() override;

private:
    bool m_isCursorLocked;
};

} // namespace Fern
