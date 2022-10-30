//
// Created by Admin on 17.02.2022.
//

#import <UIKit/UIKit.h>
#import "Platform/WindowImpl/UIKitWindow/Controller/View/WonderView.hpp"

#include "UIKitWindow.hpp"
#include "Panda/Application/PlatformData.hpp"

namespace Panda {

UIKitWindow::UIKitWindow() {
    dispatch_async(dispatch_get_main_queue(), ^{
        size = Size(
            UIScreen.mainScreen.bounds.size.width,
            UIScreen.mainScreen.bounds.size.height
        );
    });
}

bool UIKitWindow::isFullScreen() {
    return true;
}

void UIKitWindow::setFullScreen(bool isFullScreen) {}

void UIKitWindow::pollEvents() {}

void UIKitWindow::toggleCursorLock() {}

bool UIKitWindow::isCursorLocked() { return true; }

void UIKitWindow::setEventQueue(EventQueue *eventQueue) {
    dispatch_async(dispatch_get_main_queue(), ^{
        WonderView *uiView = (__bridge WonderView*) PlatformData::get()->nativeWindowHandle;
        uiView.eventQueue = eventQueue;
    });
}

Size UIKitWindow::getSize() {
    return size;
}

}
