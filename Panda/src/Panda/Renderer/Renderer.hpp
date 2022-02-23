//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"

namespace Panda {

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void initialize() = 0;
    virtual void setViewportSize(GSize size) = 0;
    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear() = 0;
    virtual void draw(int elementsCount) = 0;
    virtual void drawIndexed(unsigned int indicesCount) = 0;
    virtual void setLineWidth(float width) = 0;
    virtual void drawLines(int pointsCount) = 0;
    virtual void checkForErrors() = 0;
};

} // namespace Panda
