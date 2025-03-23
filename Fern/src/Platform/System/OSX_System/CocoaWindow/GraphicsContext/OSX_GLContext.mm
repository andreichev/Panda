#import "OSX_GLContext.h"

#import <Foundation/Logger.hpp>
#import <Cocoa/Cocoa.h>
#import <glad/glad.h>

typedef void (*glproc)(void);

static CFBundleRef s_framework;

static glproc getProcAddress(const char* procname) {
    CFStringRef symbolName = CFStringCreateWithCString(kCFAllocatorDefault,
                                                       procname,
                                                       kCFStringEncodingASCII);
    glproc symbol = (glproc) CFBundleGetFunctionPointerForName(s_framework, symbolName);
    CFRelease(symbolName);
    return symbol;
}

OSX_GLContext::OSX_GLContext(NSView* view) {
    if(s_framework == NULL) {
        s_framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
        if (s_framework == NULL) {
            LOG_CRITICAL("ERROR: Failed to locate OpenGL framework");
            return;
        }
    }

    NSOpenGLPixelFormatAttribute attributes[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFAClosestPolicy,
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 101000
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
#else
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
#endif
        0
    };

    NSOpenGLPixelFormat* pixelFormat =
        [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    if (pixelFormat == nil) {
        LOG_CRITICAL("Failed to find a suitable pixel format");
        return;
    }
    m_context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat
                                         shareContext:nil];
    [pixelFormat release];
    if(m_context == nil) {
        LOG_CRITICAL("ERROR: Failed to create OpenGL context");
        return;
    }
    [view setWantsBestResolutionOpenGLSurface:YES];
    [m_context setView: view];
    // Enable v-sync
    int interval = 1;
    [m_context setValues:&interval forParameter:NSOpenGLContextParameterSwapInterval];
}

void OSX_GLContext::setCurrent() {
    [m_context makeCurrentContext];
    if (gladLoadGLLoader((GLADloadproc)getProcAddress) == false) {
        LOG_CRITICAL("Failed to initialize GLAD");
    }
}

void OSX_GLContext::releaseDrawingContext() {
    [NSOpenGLContext clearCurrentContext];
}

void OSX_GLContext::swapBuffers() {
    [m_context flushBuffer];
}

void OSX_GLContext::update() {
    [m_context update];
}

uint32_t OSX_GLContext::getDefaultFrameBufferId() {
    return 0;
}
