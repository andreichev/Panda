//
// Created by Admin on 17.02.2022.
//

#include "UIKitWindow.hpp"

namespace Panda {

void UIKitWindow::initialize(const char *title, GSize size, bool isFullscreen) {}

UIKitWindow::~UIKitWindow(){}

double UIKitWindow::getTime(){
    return 0;
}

bool UIKitWindow::isShouldClose(){
    return false;
}

void UIKitWindow::setShouldClose(bool flag){}

bool UIKitWindow::isFullScreen(){
    return false;
}

void UIKitWindow::setFullScreen(bool isFullScreen){}

GSize UIKitWindow::getWindowSize(){
    return {100, 100};
}

void UIKitWindow::swapBuffers(){}

void* UIKitWindow::getNativeWindow(){
    return NULL;
}

void* UIKitWindow::getProcAddressFuncPointer(){
    return NULL;
}

}
