#pragma once

#include "../../../utils/logger.h"
#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* Convert shader type enum to string. */
const char* ShaderTypeToString(GLenum shaderType) {
    switch (shaderType) {
        case GL_VERTEX_SHADER:
            return "Vertex";
        case GL_FRAGMENT_SHADER:
            return "Fragment";
        case GL_GEOMETRY_SHADER:
            return "Geometry";
        case GL_TESS_CONTROL_SHADER:
            return "Tessellation Control";
        case GL_TESS_EVALUATION_SHADER:
            return "Tessellation Evaluation";
        case GL_COMPUTE_SHADER:
            return "Compute";
        default:
            return "Unknown";
    }
}

/* Read a shader file and return the source code as GLchar*. */
static GLchar* ReadShaderSource(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        Logger::LogError("Failed to open shader file: %s", path);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    GLchar* source = (GLchar*)malloc(fileSize + 1);
    if (!source) {
        Logger::LogError("Memory allocation error");
        fclose(file);
        return nullptr;
    }

    fread(source, 1, fileSize, file);
    fclose(file);
    source[fileSize] = '\0'; // Null-terminate the string

    return source;
}

/* Compile a shader from source code and return the shader's ID. */
static GLuint CompileShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        Logger::LogError("Shader compilation error: %s", infoLog);
        
        glDeleteShader(shader);
        return 0;
    }
    
    Logger::LogDebug("%s shader compiled successfully", ShaderTypeToString(shaderType));
    return shader;
}

/* Create a shader program, attach shaders and return the program's ID. */
static GLuint CreateShaderProgram(GLuint count, ...) {
    GLuint program = glCreateProgram();

    va_list shaderList;
    va_start(shaderList, count);

    for (GLuint i = 0; i < count; ++i) {
        GLuint shader = va_arg(shaderList, GLuint);
        glAttachShader(program, shader);
    }

    va_end(shaderList);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);

        Logger::LogError("Shader program linking error: %s", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    // Delete shaders after linking
    va_start(shaderList, count);
    for (GLuint i = 0; i < count; ++i) {
        GLuint shader = va_arg(shaderList, GLuint);
        glDeleteShader(shader);
    }
    va_end(shaderList);

    return program;
}