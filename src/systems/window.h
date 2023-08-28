#pragma once

#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../utils/frametimer.h"
#include "../utils/logger.h"

#include <functional>

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();

    void Run(std::function<void(float deltaTime)> render);

    void ToggleFullscreen()
    {
        int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        Logger::LogDebug("Fullscreen: %s", maximized == 0 ? "true" : "false");
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

    // Cycle between wireframe and filled mode
    void CycleRenderMode()
    {
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        
        if (polygonMode[0] == GL_FILL)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        Logger::LogDebug("Polygon mode: %s", polygonMode[0] == GL_FILL ? "GL_FILL" : "GL_LINE");
    }

    GLFWwindow *GetGLFWWindow() const
    {
        return window;
    }

private:
    int width, height;
    const char *title;
    FrameTimer frameTimer;

    GLint bufferWidth, bufferHeight;
    GLFWwindow *window;

    int windowedPosX, windowedPosY;
};
