#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

// local
#include "../../systems/window/window_context.h"
#include "../../systems/shader/shader.h"
#include "../../systems/camera/perspective_camera.h"

class TranslationParams{
public:
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    float translationSpeed = 2.0f;
};

class ScaleParams{
public:
    float scaleFactor = 1.0f;
    float scaleSpeed = 1.5f;
    glm::vec2 scaleBounds = glm::vec2(0.1f, 2.0f);
};

class RotationParams{
public:
    float rotationAngle = 0.0f;
    float rotationSpeed = 10.0f;
};

class CubeDemo
{
private:
    float vertices[24] = {
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
       -0.5f, 0.5f, -0.5f,
       -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
       -0.5f, 0.5f, 0.5f,
    };
    
    // (CCW)
    unsigned int indices[36] = {
        0, 1, 2, 2, 3, 0, // Front 
        4, 5, 6, 6, 7, 4, // Back 
        1, 5, 6, 6, 2, 1, // Right 
        0, 4, 7, 7, 3, 0, // Left 
        3, 2, 6, 6, 7, 3, // Top 
        0, 1, 5, 5, 4, 0  // Bottom
    };

    float colors[24] = {
        1.0f, 0.0f, 0.0f, // Red
        0.0f, 1.0f, 0.0f, // Green
        0.0f, 0.0f, 1.0f, // Blue
        1.0f, 1.0f, 0.0f, // Yellow
        0.0f, 1.0f, 0.0f, // Green
        0.0f, 1.0f, 1.0f, // Cyan
        1.0f, 1.0f, 1.0f, // White
        0.5f, 0.5f, 0.5f  // Gray
    };



    PerspectiveCamera camera;

    const char *vertexSource =
#include "../../resources/shaders/CubeDemo/shader.vert"
;
    const char *fragSource =
#include "../../resources/shaders/CubeDemo/shader.frag"
;

    Shader cubeShader = Shader()
                        .SetVertexSource(vertexSource)
                        .SetFragmentSource(fragSource)
                        .Compile();

    glm::vec4 cubeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    
    glm::vec3 cubeTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cubeRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);

    unsigned int VBO, VAO, EBO;            

public:
    CubeDemo(WindowContext &context);

    glm::vec4 clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);

    TranslationParams translationParams;
    ScaleParams scaleParams;
    RotationParams rotationParams;
    GLFWwindow *window;

    std::function<void()> uiUpdate;
    std::function<void (float deltaTime)> gameUpdate;
};
