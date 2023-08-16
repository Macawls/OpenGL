#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Compile a shader from source code and return the shader's ID. */
static GLuint compileShader(GLenum shaderType, const GLchar* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Shader compilation error: %s\n", infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

/* Link a shader program from vertex and fragment shader IDs and return the program's ID. */
static GLuint linkShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Shader program linking error: %s\n", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    // delete after linking
    glDeleteShader(vertexShader);
    return program;
}