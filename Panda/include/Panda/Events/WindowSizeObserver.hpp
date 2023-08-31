//
// Created by Admin on 08.02.2022.
//

#pragma once

#include "Panda/Base/Base.hpp"

namespace Panda {

class WindowSizeObserver {
public:
    virtual void windowSizeChanged(Size size) = 0;
};

} // namespace Panda
