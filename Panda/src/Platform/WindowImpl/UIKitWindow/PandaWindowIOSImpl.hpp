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
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    void pollEvents() override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void setEventQueue(EventQueue *eventQueue) override;
    Size getSize() override;
    Size getDpi() override;

private:
    Size m_size;
    Size m_dpi;
};

} // namespace Panda
