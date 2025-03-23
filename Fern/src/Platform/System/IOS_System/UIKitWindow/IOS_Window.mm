//
// Created by Admin on 17.02.2022.
//

#import <UIKit/UIKit.h>

#import "WonderView.h"
#import "IOS_Window.hpp"

namespace Fern {

IOS_Window::IOS_Window() {
    m_size = Size(
                  UIScreen.mainScreen.bounds.size.width,
                  UIScreen.mainScreen.bounds.size.height
                  );
    m_dpi = Size(
                 UIScreen.mainScreen.nativeScale,
                 UIScreen.mainScreen.nativeScale
                 );
}

}
