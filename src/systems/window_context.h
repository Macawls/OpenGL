#pragma once

#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../utils/logger.h"

#include <functional>

struct RenderConfig
{
    bool fullscreen;
    bool useLines;
};


class WindowContext
{
public:
    WindowContext(int width, int height, const char *title);
    // WindowContext(ContextConfig config);
    
    ~WindowContext();

    void BeginLoop();
    
    RenderConfig renderConfig;

    void SetUpdate(std::function<void(float deltaTime)> func)
    {
        this->update = func;
    }

    void ToggleFullscreen()
    {
        int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        Logger::LogDebug("Fullscreen: %s", maximized == 0 ? "true" : "false");
        if (maximized)
        {
            glfwRestoreWindow(window);
            renderConfig.fullscreen = false;
            return;
        }
        glfwMaximizeWindow(window);
        renderConfig.fullscreen = true;
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
            renderConfig.useLines = true;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            renderConfig.useLines = false;
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

    std::function<void(float deltaTime)> update;

    GLint bufferWidth, bufferHeight;
    GLFWwindow *window;

    void ImGUIBegin();
    void ImGUIEnd();
};
