#define GLEW_STATIC  // BEFORE including glew.h!
#include <GL/glew.h> // always put glew.h before glfw3.h
#include "shader.h"


Shader::Shader(const char* vertSource, const char* fragsSource)
{
    vert = CompileShader(GL_VERTEX_SHADER, vertSource);
    frag = CompileShader(GL_FRAGMENT_SHADER, fragsSource);
    
    ID = CreateShaderProgram(2, vert, frag);
}

Shader& Shader::Use() 
{ 
    glUseProgram(ID);
    return *this; 
}

Shader& Shader::SetVertexSource(const char* vertSource)
{
    vert = CompileShader(GL_VERTEX_SHADER, vertSource);
    return *this;
}

Shader& Shader::SetFragmentSource(const char* fragsSource)
{
    frag = CompileShader(GL_FRAGMENT_SHADER, fragsSource);
    return *this;
}

Shader& Shader::Compile()
{
    ID = CreateShaderProgram(2, vert, frag);
    return *this;
}

Shader& Shader::SetBool(const std::string &name, bool value)
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    return *this;
}

Shader& Shader::SetInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    return *this;
}

Shader& Shader::SetFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    return *this; 
}

Shader& Shader::SetVec2(const std::string &name, const glm::vec2 &value)
{ 
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    return *this; 
}

Shader& Shader::SetVec3(const std::string &name, const glm::vec3 &value)
{ 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    return *this; 
}

Shader& Shader::SetVec4(const std::string &name, const glm::vec4 &value)
{ 
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    return *this; 
}

Shader& Shader::SetMat2(const std::string &name, const glm::mat2 &mat)
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this; 
}

Shader& Shader::SetMat3(const std::string &name, const glm::mat3 &mat)
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this; 
}

Shader& Shader::SetMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    return *this; 
}