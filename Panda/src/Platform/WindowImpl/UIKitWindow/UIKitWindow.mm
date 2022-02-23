//
// Created by Admin on 17.02.2022.
//

#include "UIKitWindow.hpp"
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <OpenGLES/ES3/gl.h>

namespace Panda {

GLuint colorRenderBuffer;
GLuint depthRenderBuffer;
GLuint frameBuffer;
EAGLContext* context;

void UIKitWindow::initialize(const char *title, GSize size, bool isFullscreen) {
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    [EAGLContext setCurrentContext:context];
    
    GLsizei width = size.width;
    GLsizei height = size.height;
    
    glGenRenderbuffers(1, &colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    glViewport(0, 0, width, height);
    glClearColor(1.f, 0.5f, 0.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

UIKitWindow::~UIKitWindow(){}

double UIKitWindow::getTime(){
    return ([[NSDate date] timeIntervalSince1970]);
}

bool UIKitWindow::isShouldClose(){
    return false;
}

void UIKitWindow::setShouldClose(bool flag){}

bool UIKitWindow::isFullScreen(){
    return false;
}

void UIKitWindow::setFullScreen(bool isFullScreen){}

GSize UIKitWindow::getWindowSize(){
    return {100, 100};
}

void UIKitWindow::swapBuffers(){}

void* UIKitWindow::getNativeWindow(){
    return NULL;
}

void* UIKitWindow::getProcAddressFuncPointer(){
    return NULL;
}

}
