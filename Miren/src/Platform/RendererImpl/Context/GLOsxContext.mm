#import "GLOsxContext.hpp"
#import "Miren/PlatformData.hpp"

#import <Foundation/Logger.hpp>
#import <Cocoa/Cocoa.h>
#import <glad/glad.h>

namespace Miren {

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

void GLOsxContext::create() {
    if(s_framework == NULL) {
        s_framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
        if (s_framework == NULL) {
            LOG_CRITICAL("ERROR: Failed to locate OpenGL framework");
            return;
        }
    }

    NSWindow* window = (NSWindow*) PlatformData::get()->nativeWindowHandle;

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
    context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat
                                         shareContext:nil];
    if(context == nil) {
        LOG_CRITICAL("ERROR: Failed to create OpenGL context");
        return;
    }
    [[window contentView] setWantsBestResolutionOpenGLSurface:YES];
    [context setView: [window contentView]];
    // Enable v-sync
    int interval = 1;
    [context setValues:&interval forParameter:NSOpenGLContextParameterSwapInterval];
}

void GLOsxContext::setCurrent() {
    [context makeCurrentContext];
    if (gladLoadGLLoader((GLADloadproc)getProcAddress) == false) {
        LOG_CRITICAL("Failed to initialize GLAD");
    }
}

void GLOsxContext::update() {
    [context update];
}

void GLOsxContext::flip() {
    [context flushBuffer];
}

uint32_t GLOsxContext::getDefaultFrameBufferId() {
    return 0;
}

} // namespace Miren
