#pragma once

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Miren {

const char *getGLErrorStr(GLenum err);

#define _GL_CALL(call, assert)                                                                     \
    {                                                                                              \
        call;                                                                                      \
        const GLenum err = glGetError();                                                           \
        assert(GL_NO_ERROR == err, #call "; OPENGL ERROR {}", getGLErrorStr(err));                 \
    }

#define GL_CALL(call) _GL_CALL(call, PND_ASSERT_F)

} // namespace Miren