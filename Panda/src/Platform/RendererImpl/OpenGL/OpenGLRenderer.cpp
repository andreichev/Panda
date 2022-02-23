//
// Created by Admin on 11.02.2022.
//
#include "pndpch.hpp"
#include "OpenGLRenderer.hpp"
#include "Panda/Application/Application.hpp"

#ifdef PND_PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PND_PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Panda {

void OpenGLRenderer::initialize() {
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
    printf("CLEAR COLOR: %f, %f, %f, %f\n", r, g, b, a);
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

const char *getGLErrorStr(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
        // case GL_STACK_OVERFLOW:    return "Stack overflow";
        // case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        default:
            return "Unknown error";
    }
}

void OpenGLRenderer::checkForErrors() {
    while (true) {
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;
        PND_ERROR("OPENGL: {}", getGLErrorStr(err));
    }
}

} // namespace Panda
