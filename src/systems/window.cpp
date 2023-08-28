#include "window.h"
#include <cstdio>
#include "../utils/logger.h"
#include "../utils/frameTimer.h"


Window::Window(int width, int height, const char *title) : width(width), height(height), title(title)
{
    frameTimer = FrameTimer();
    
    if (!glfwInit())
    {
        Logger::Log(Logger::LogPriority::Error, "Failed to initialize GLFW");
    }
    Logger::Log("Initialized GLFW");

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
    {
        Logger::Log(Logger::LogPriority::Error, "Failed to create GLFW window");
    }
    Logger::Log("Created GLFW window");

    // Get buffer size information
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set opengl context, before glew init
    glfwMakeContextCurrent(window);

    // Make sure viewport updates when window is resized
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
    { 
        glViewport(0, 0, width, height); 
    });

    // Set viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Set user pointer to this class, so we can access it in callbacks
    glfwSetWindowUserPointer(window, this);

    // Enable VSync
    //glfwSwapInterval(1);
    
    GLenum err = glewInit();
    
    if (err != GLEW_OK)
    {
        Logger::LogError("GLEW Init Failed, Error: %s", glewGetErrorString(err));
        glfwTerminate();
        return;
    }

    Logger::Log("OpenGL version: %s", glGetString(GL_VERSION));
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::Run(std::function<void(float deltaTime)> render)
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        render(frameTimer.GetDeltaTime());
        glfwSwapBuffers(window);
        
        frameTimer.UpdateWindowTitleWithFPS(window, title);
    }
}