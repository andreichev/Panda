//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/IRenderer.hpp"

namespace Panda {

class OpenGLRenderer : public IRenderer {
public:
    void initialize() override;
    void setViewportSize(GSize size) override;
    void setClearColor(float r, float g, float b, float a) override;
    void clear() override;
    void draw(int elementsCount) override;
    void drawIndexed(unsigned int indicesCount) override;
    void setLineWidth(float width) override;
    void drawLines(int pointsCount) override;

private:
    float lineWidthRange[2];
};

} // namespace Panda