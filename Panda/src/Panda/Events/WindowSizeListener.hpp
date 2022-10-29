//
// Created by Admin on 08.02.2022.
//

#pragma once

#include <Foundation/Size.hpp>

namespace Panda {

class WindowSizeListener {
public:
    virtual void windowSizeChanged(UISize size) = 0;
};

} // namespace Panda
