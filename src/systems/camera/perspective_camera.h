#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../window/window_context.h"
#include <GLFW/glfw3.h>

// Notes: aspect ratio of window size = stretched, aspect ratio of framebuffer size = no stretching
// TODO: should rather use events for notifying about window resize, and then update the camera

class PerspectiveCamera
{
public:
    PerspectiveCamera() = default;

    PerspectiveCamera(WindowContext &windowContext)
    {
        context = &windowContext;
        window = context->GetGLFWWindow();

        UpdateAspectRatio();

        UpdateProjectionMatrix();
        UpdateViewMatrix();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 GetProjectionMatrix()
    {
        return projectionMatrix;
    }

    void OnUpdate(float deltaTime)
    {
        UpdateProjectionMatrix();
        // updateViewMatrix();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += speed * deltaTime * front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position -= speed * deltaTime * front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position -= glm::normalize(glm::cross(front, up)) * speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += glm::normalize(glm::cross(front, up)) * speed * deltaTime;
    }

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float fov = 60.0f;
    float speed = 3.0f;
    float sensitivity = 0.05f;

private:
    float aspectRatio;
    float nearClip = 0.1f;
    float farClip = 100.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;

    WindowContext *context;
    GLFWwindow *window;

    glm::mat4 projectionMatrix;

    void UpdateProjectionMatrix()
    {
        UpdateAspectRatio();
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    }

    void UpdateViewMatrix()
    {
        // Recalculate the front vector
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        // Recalculate the right and up vectors
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));
    }

    void UpdateAspectRatio()
    {
        aspectRatio = context->GetFrameBufferAspectRatio();
    }
};