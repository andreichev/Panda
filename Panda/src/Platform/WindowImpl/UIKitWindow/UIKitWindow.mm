//
// Created by Admin on 17.02.2022.
//

#include "UIKitWindow.hpp"
#include "Panda/Application/PlatformData.hpp"

#import <UIKit/UIKit.h>
#import <OpenGLES/ES3/gl.h>

namespace Panda {

void UIKitWindow::initialize(const char *title, GSize size, bool isFullscreen) {}

bool UIKitWindow::isFullScreen(){
    return true;
}

void UIKitWindow::setFullScreen(bool isFullScreen){}

void UIKitWindow::pollEvents() {}

void UIKitWindow::toggleCursorLock() {}

bool UIKitWindow::isCursorLocked() { return true; }

}
