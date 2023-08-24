#include "window.h"
#include <cstdio>
#include "../utils/logger.h"
#include "../utils/frameTimer.h"


Window::Window(int width, int height, const char *title) : width(width), height(height), title(title)
{
    frameTimer = FrameTimer();
    
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
    }

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
    {
        printf("Failed to create GLFW window\n");
    }

    // Get buffer size info
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // window context
    glfwMakeContextCurrent(window);

    // Setup standard callbacks
    InitCallbacks();

    // Set viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Set user pointer to this class, so we can access it in callbacks
    glfwSetWindowUserPointer(window, this);

    // Enable VSync
    // glfwSwapInterval(1);
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        Logger::Log(Logger::LogPriority::Error, "Error: %s", glewGetErrorString(err));
        glfwTerminate();
    }

    Logger::Log("OpenGL version: %s", glGetString(GL_VERSION));
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::InitCallbacks()
{
    // Make sure viewport is resized when window is resized
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });
}

void Window::Run(std::function<void(float deltaTime)> render)
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        float delta = frameTimer.GetDeltaTime();
        render(delta);
        glfwSwapBuffers(window);
        frameTimer.UpdateWindowTitleWithFPS(window, title);
    }
}