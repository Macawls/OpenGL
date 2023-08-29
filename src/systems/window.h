#pragma once

#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../utils/logger.h"

#include <functional>

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();

    void BeginLoop();

    bool fullscreen = false;
    bool useLines = false;

    void SetPrimaryUpdate(std::function<void(float deltaTime)> func)
    {
        this->primary = func;
    }

    void SetSecondaryUpdate(std::function<void()> func)
    {
        this->secondary = func;
    }

    void ToggleFullscreen()
    {
        int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        Logger::LogDebug("Fullscreen: %s", maximized == 0 ? "true" : "false");
        if (maximized)
        {
            glfwRestoreWindow(window);
            fullscreen = false;
            return;
        }
        glfwMaximizeWindow(window);
        fullscreen = true;
    }

    void Close()
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Cycle between wireframe and filled mode
    void CycleRenderMode()
    {
        GLint mode[2];
        glGetIntegerv(GL_POLYGON_MODE, mode);
        
        if (mode[0] == GL_FILL)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            useLines = true;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            useLines = false;
        }

        Logger::LogDebug("Polygon mode: %s", mode[0] == GL_FILL ? "GL_FILL" : "GL_LINE");
    }

    GLFWwindow *GetGLFWWindow() const
    {
        return window;
    }

private:
    int width, height;
    const char *title;

    std::function<void(float deltaTime)> primary;
    std::function<void()> secondary;

    GLint bufferWidth, bufferHeight;
    GLFWwindow *window;

    int windowedPosX, windowedPosY;
};
