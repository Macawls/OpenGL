#pragma once
#include "shader_utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;
    GLuint vert;
    GLuint frag;

    Shader() {}
    Shader(const char* vertSource, const char* fragsSource);

    Shader& SetVertexSource(const char* vertSource);
    Shader& SetFragmentSource(const char* fragsSource);
    
    Shader& Compile();
    
    Shader& Use();

    // Primitive
    Shader& SetBool(const std::string &name, bool value);
    Shader& SetInt(const std::string &name, int value);
    Shader& SetFloat(const std::string &name, float value);
    
    // Vector
    Shader& SetVec2(const std::string &name, const glm::vec2 &value);
    Shader& SetVec3(const std::string &name, const glm::vec3 &value);
    Shader& SetVec4(const std::string &name, const glm::vec4 &value);
    
    // Matrix
    Shader& SetMat2(const std::string &name, const glm::mat2 &mat);
    Shader& SetMat3(const std::string &name, const glm::mat3 &mat);
    Shader& SetMat4(const std::string &name, const glm::mat4 &mat);
};