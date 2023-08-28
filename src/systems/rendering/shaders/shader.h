#pragma once

#include "utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertPath, const char* fragPath)
    {
        GLuint vert = CompileShader(GL_VERTEX_SHADER, vertPath);
        GLuint frag = CompileShader(GL_FRAGMENT_SHADER, fragPath);
        
        ID = CreateShaderProgram(2, vert, frag);
    }
   
    Shader Use() 
    { 
        glUseProgram(ID);
        return *this; 
    }

    Shader SetBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        return *this;
    }
   
    Shader SetInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        return *this;
    }
    
    Shader SetFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        return *this; 
    }

    Shader SetVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        return *this; 
    }

    Shader SetVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        return *this; 
    }

    Shader SetVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
        return *this; 
    }

    Shader SetMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        return *this; 
    }

    Shader SetMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        return *this; 
    }
};