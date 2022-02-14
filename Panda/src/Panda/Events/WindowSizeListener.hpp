//
// Created by Admin on 08.02.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"

namespace Panda {

class WindowSizeListener {
public:
    virtual void windowSizeChanged(GSize size) = 0;
};

} // namespace Panda
