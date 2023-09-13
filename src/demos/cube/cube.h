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

// https://github.com/c2d7fa/opengl-cube/blob/master/main.c

class CubeDemo
{
private:
    float vertices[24] = {
        // Front face
        0.5,  0.5,  0.5,
       -0.5,  0.5,  0.5,
       -0.5, -0.5,  0.5,
        0.5, -0.5,  0.5,

        // Back face
        0.5,  0.5, -0.5,
       -0.5,  0.5, -0.5,
       -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
    };
    
    
    unsigned int indices[36] = {
        // Front
        0, 1, 2,
        2, 3, 0,

        // Right
        0, 3, 7,
        7, 4, 0,

        // Bottom
        2, 6, 7,
        7, 3, 2,

        // Left
        1, 5, 6,
        6, 2, 1,

        // Back
        4, 7, 6,
        6, 5, 4,

        // Top
        5, 1, 0,
        0, 4, 5,
    };

    float colors[24] = {
        1.0, 0.4, 0.6,
        1.0, 0.9, 0.2,
        0.7, 0.3, 0.8,
        0.5, 0.3, 1.0,

        0.2, 0.6, 1.0,
        0.6, 1.0, 0.4,
        0.6, 0.8, 0.8,
        0.4, 0.8, 0.8,
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
