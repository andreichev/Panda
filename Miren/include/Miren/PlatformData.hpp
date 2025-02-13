//
// Created by Admin on 18.02.2022.
//

#pragma once

#include "Miren/GraphicsContext.hpp"

namespace Miren {

// Указатели на главные элементы платформы (окно, контекст для рендера и т. д.)
class PlatformData {
public:
    static PlatformData *get();
    GraphicsContext *graphicsContext = nullptr;
    void *layer = nullptr;
    void *nativeWindowHandle = nullptr;
};

} // namespace Miren