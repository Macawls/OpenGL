#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

// local
#include "../systems/window_context.h"
#include "../systems/rendering/shaders/shader.h"

class TranslationParams{
public:
    float translationX = 0.0f;
    float translationY = 0.0f;
    float translationXMax = 0.5f;
    float translationYMax = 0.5f;
    float translationSpeed = 2.0f;
};

class ScaleParams{
public:
    float scaleFactor = 1.0f;
    float scaleSpeed = 1.5f;
    float scaleMax = 2.0f;
    float scaleMin = 0.1f;
};

class RotationParams{
public:
    float rotationAngle = 0.0f;
    float rotationSpeed = 10.0f;
};

class TriangleDemo
{
private:
    float vertices[9] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    float colours[9] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    const char *vertexSource =
#include "../resources/shaders/TriangleDemo/shader.vert"
;
    const char *fragSource =
#include "../resources/shaders/TriangleDemo/shader.frag"
;

    Shader triangleShader = Shader()
                        .SetVertexSource(vertexSource)
                        .SetFragmentSource(fragSource)
                        .Compile();

    const char* renderer;

public:
    TriangleDemo(WindowContext &context);

    glm::vec4 clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);

    TranslationParams translationParams;
    ScaleParams scaleParams;
    RotationParams rotationParams;
    GLFWwindow *window;

    std::function<void()> uiUpdate;
    std::function<void (float deltaTime)> gameUpdate;
};
