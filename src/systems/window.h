#pragma once
#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional> // std::function

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void Run(std::function<void()> renderLoop);

    void Close();
    void ToggleFullscreen();
    
    GLFWwindow* GetGLFWWindow();

private:
    int width, height;
    const char* title;
    GLFWwindow* window;
    bool isFullscreen;
    int windowedPosX, windowedPosY;
};
