#pragma once
#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional> // std::function

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();

    void Run(std::function<void()> render);

    void ToggleFullscreen()
    {
        int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        if (maximized)
        {
            glfwRestoreWindow(window);
            return;
        }
        glfwMaximizeWindow(window);
    }
    void Close()
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    GLFWwindow *GetGLFWWindow()
    {
        return window;
    }

private:
    int width, height;
    const char *title;

    GLint bufferWidth, bufferHeight;
    GLFWwindow *window;

    bool isFullscreen;
    int windowedPosX, windowedPosY;

    void InitCallbacks();
};
