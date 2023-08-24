#include "window.h"
#include <cstdio>
#include "../utils/utils.h"

Window::Window(int width, int height, const char *title) : width(width), height(height), title(title)
{
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

    // Set user pointer to this class
    glfwSetWindowUserPointer(window, this);

    // Enable VSync
    // glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
    }
}

Window::~Window()
{
    glfwTerminate();
}

void Window::InitCallbacks()
{
    // Make sure viewport is resized when window is resized
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height){ 
            glViewport(0, 0, width, height); 
        });

    // Standard controls
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods){
            Window* w = (Window*)glfwGetWindowUserPointer(window);
            
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                w->Close();
            }
                
            if (key == GLFW_KEY_F && action == GLFW_PRESS){
                w->ToggleFullscreen();
            }
        });
}

void Window::Close()
{
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void Window::ToggleFullscreen(){
    int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
    if (maximized){
        glfwRestoreWindow(window);
        return;
    }
    glfwMaximizeWindow(window);
}

void Window::Run(std::function<void()> render)
{
    double lastTime = glfwGetTime();
    int frameCount = 0;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        render();
        glfwSwapBuffers(window);
        updateTitleWithFPS(window, frameCount, lastTime, title);
        frameCount++;
    }
}

GLFWwindow *Window::GetGLFWWindow()
{
    return window;
}