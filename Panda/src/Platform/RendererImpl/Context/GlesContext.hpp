//
// Created by Admin on 11.02.2022.
//

#pragma once
#include "Panda/Renderer/GraphicsContext.hpp"

#import <OpenGLES/ES3/gl.h>

namespace Panda {

class GlesContext: public GraphicsContext {
    void create(int width, int height) override;
    void swapBuffers() override;

private:
    EAGLContext* context;
};

}