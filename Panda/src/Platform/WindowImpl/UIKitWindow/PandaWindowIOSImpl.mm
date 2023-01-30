//
// Created by Admin on 17.02.2022.
//

#import <UIKit/UIKit.h>
#include <Miren/PlatformData.hpp>

#import "Platform/WindowImpl/UIKitWindow/Controller/View/WonderView.hpp"
#include "PandaWindowIOSImpl.hpp"

namespace Panda {

PandaWindowIOSImpl::PandaWindowIOSImpl() {
    m_size = Size(
        UIScreen.mainScreen.bounds.size.width,
        UIScreen.mainScreen.bounds.size.height
    );
    m_dpi = Size(
        UIScreen.mainScreen.nativeScale,
        UIScreen.mainScreen.nativeScale
    );
}

bool PandaWindowIOSImpl::isFullScreen() {
    return true;
}

void PandaWindowIOSImpl::setFullScreen(bool isFullScreen) {}

void PandaWindowIOSImpl::pollEvents() {}

void PandaWindowIOSImpl::toggleCursorLock() {}

bool PandaWindowIOSImpl::isCursorLocked() { return true; }

void PandaWindowIOSImpl::setEventQueue(EventQueue *eventQueue) {
    WonderView *uiView = (__bridge WonderView*)
        Miren::PlatformData::get()->nativeWindowHandle;
    uiView.eventQueue = eventQueue;
}

Size PandaWindowIOSImpl::getSize() {
    return m_size;
}

Size PandaWindowIOSImpl::getDpi() {
    return m_dpi;
}

}
