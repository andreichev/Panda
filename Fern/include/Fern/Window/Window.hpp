//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Fern/Base/Base.hpp"
#include "Fern/Window/Cursor.hpp"
#include "Fern/GraphicsContext/GraphicsContext.hpp"

namespace Fern {

class Window {
public:
    virtual ~Window() = default;
    virtual void setTitle(const char *title) = 0;
    virtual WindowState getState() = 0;
    virtual void setState(WindowState state) = 0;
    virtual void setModifiedState(bool isUnsavedChanges) = 0;
    virtual bool getModifiedState() = 0;
    virtual void setResizable(bool isResizable) = 0;
    virtual void setSize(Size size) = 0;
    virtual Size getSize() = 0;
    virtual Size getDpi() = 0;
    virtual Vec2 getCursorPosition() = 0;
    virtual bool setProgressBar(float progress) = 0;
    virtual bool endProgressBar() = 0;
    // Graphics context
    virtual GraphicsContext *getDrawingContext() = 0;
};

} // namespace Fern
