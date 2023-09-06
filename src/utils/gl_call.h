#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include "logger.h"

// Source: https://youtu.be/FBbPWSOQ0-w?feature=shared
// DISABLE_OPENGL_ERROR_CHECKING is specified for release builds
// Example to trigger an error GLCall(glEnable(GL_INVALID_ENUM));

#ifdef DISABLE_OPENGL_ERROR_CHECKING
    #define GLCall(x) x
#else
    // Macro that polls for OpenGL errors, 
    // Logs the error and terminates the program if there is one
    #define GLCall(x) \
        x;\
        CheckGLError(#x, __FILE__, __LINE__)
#endif

static const char* GetGLErrorString(GLenum error) {
    switch (error) {
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "Unknown GL error";
    }
}


static void CheckGLError(const char* function, const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) 
    {
        const char* errorString = GetGLErrorString(error);
        Logger::LogError("[OpenGL Error] %s: %s, %s:%d", function, errorString, file, line);
        
        // Force exit if there is an error
        exit(1);
    }
}