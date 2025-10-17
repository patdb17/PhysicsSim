#ifndef OPEN_GL_UTILS_H
#define OPEN_GL_UTILS_H
// OpenGlUtils.h

// C++ Standard Library includes
#include <string>

// External Libraries includes
#include <GL/glew.h>

// Project includes
#include "Logger.h"

// Compiler-specific includes
#if defined(__GNUC__) // GNU Compiler Collection (GCC) or Clang
    #include <cassert>
    #define ASSERT(x) assert(x)
#elif defined(_MSC_VER) // Microsoft Visual C++
    #define ASSERT(x) if (!(x)) __debugbreak();
#endif

// GL error macros
#define GL_CALL(x) GlClearError(); \
    x; \
    ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* functionName, const char* fileName, const unsigned int lineNumber);

static inline std::string GetGlErrorString(const unsigned int errorCode)
{
    switch (errorCode)
    {
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
        case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
        default: return "Unknown error code " + std::to_string(errorCode);
    }
}


#endif // OPEN_GL_UTILS_H