#pragma once

#include <stdio.h>
#include <GLFW/glfw3.h>

class FrameTimer
{
public:
    FrameTimer()
    {
        titleBuffer = new char[256];
    }

    ~FrameTimer()
    {
        delete[] titleBuffer;
    }

    void UpdateWindowTitleWithFPS(GLFWwindow *window, const char *customTitle, const double updateDelta = 1.0f)
    {
        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - lastTime >= updateDelta)
        {
            fps = static_cast<double>(frameCount) / (currentTime - lastTime);
            snprintf(titleBuffer, 256, "%s - %.2f FPS", customTitle, fps);
            glfwSetWindowTitle(window, titleBuffer);
            lastTime = currentTime;
            frameCount = 0;
        }
    }

    double GetDeltaTime()
    {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        return deltaTime;
    }

    double GetFPS() const
    {
        return fps;
    }

private:
    double lastFrameTime;
    double lastTime;
    int frameCount;
    double fps;

    char *titleBuffer;
};
