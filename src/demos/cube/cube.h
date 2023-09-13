#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

// local
#include "../../systems/window/window_context.h"
#include "../../systems/shader/shader.h"
#include "../../systems/camera/perspective_camera.h"
#include "../../systems/settings/render_settings.h"

class CubeDemo
{
private:

    // Cube
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

    const char *cubeVert =
#include "../../resources/shaders/CubeDemo/cube_shader.vert"
;
    const char *cubeFrag =
#include "../../resources/shaders/CubeDemo/cube_shader.frag"
;


    // Cube
    unsigned int cubeVBO, cubeVAO, cubeEBO;

    Shader cubeShader = Shader()
                        .SetVertexSource(cubeVert)
                        .SetFragmentSource(cubeFrag)
                        .Compile();
    // Cube Variables
    glm::vec4 cubeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 cubeTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cubeRotation = glm::vec3(25.0f, 45.0f, 0.0f);
    glm::vec3 cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);

    OpenGLRenderSettings RenderSettings = OpenGLRenderSettings();
    
    void InitCube();

public:
    CubeDemo(WindowContext &context);

    glm::vec4 clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);
    GLFWwindow *window;

    std::function<void()> uiUpdate;
    std::function<void (float deltaTime)> gameUpdate;
};
