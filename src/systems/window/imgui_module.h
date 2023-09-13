#pragma once
#include <GLFW/glfw3.h>

#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"

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

    // ImGui IO
    ImGuiIO *IO;
private:
    void UseTheme();
};