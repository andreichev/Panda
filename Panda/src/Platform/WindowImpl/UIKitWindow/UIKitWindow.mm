//
// Created by Admin on 17.02.2022.
//

#include "UIKitWindow.hpp"
#include "Panda/Application/PlatformData.hpp"

namespace Panda {

bool UIKitWindow::isFullScreen() {
    return true;
}

void UIKitWindow::setFullScreen(bool isFullScreen) {}

void UIKitWindow::pollEvents() {}

void UIKitWindow::toggleCursorLock() {}

bool UIKitWindow::isCursorLocked() { return true; }

}
