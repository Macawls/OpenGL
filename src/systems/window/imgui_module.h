#pragma once
#include <GLFW/glfw3.h>

class ImGUIModule
{
public:
    ImGUIModule();
    ~ImGUIModule();

    // Creates ImGui context, requires glsl version, e.g. "#version 330"
    bool Init(const char *glsl_version, GLFWwindow *glfwWindow, float fontSize);

    // Run before imgui calls
    void BeginFrame();

    // Run after imgui calls
    void EndFrame();
private:
    void UseTheme();
};