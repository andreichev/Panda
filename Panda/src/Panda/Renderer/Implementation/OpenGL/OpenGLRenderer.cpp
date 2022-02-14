//
// Created by Admin on 11.02.2022.
//
#include "pndpch.hpp"
#include "OpenGLRenderer.hpp"
#include "Panda/Application/Application.hpp"

#include <glad/glad.h>

namespace Panda {

void OpenGLRenderer::initialize() {
    void *getProcAddressFuncPointer = Application::get().getWindow().getProcAddressFuncPointer();
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(getProcAddressFuncPointer))) {
        PND_CRITICAL("Failed to initialize GLAD");
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::setViewportSize(GSize size) {
    glViewport(0, 0, (int)size.width, (int)size.height);
}

void OpenGLRenderer::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGLRenderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::draw(int elementsCount) {
    glDrawArrays(GL_TRIANGLES, 0, elementsCount);
}

void OpenGLRenderer::drawIndexed(unsigned int indicesCount) {
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderer::setLineWidth(float width) {
    if (width > lineWidthRange[1] || width < lineWidthRange[0]) {
        return;
    }
    glLineWidth(width);
}

void OpenGLRenderer::drawLines(int pointsCount) {
    glDrawArrays(GL_LINES, 0, pointsCount);
}

} // namespace Panda
