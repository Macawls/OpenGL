#pragma once

#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../utils/logger.h"
#include "imgui_module.h"

#include <functional>

#define DEFAULT_FONT_SIZE 20.0f

struct RenderConfig
{
    bool fullscreen;
    bool useLines;
};


class WindowContext
{
public:
    WindowContext(int width, int height, const char *title, float imguiFontSize = DEFAULT_FONT_SIZE);
    ~WindowContext();

    // Begins the main loop, update function is called every frame
    void BeginLoop();
    
    RenderConfig renderConfig = {false, false};

    void SetUpdate(std::function<void(float deltaTime)> func) { this->update = func; }
    GLFWwindow *GetGLFWWindow() const { return window; }

    // Cycle between wireframe and filled mode
    void CycleRenderMode();
    void ToggleFullscreen();
    void Close();

private:
    GLint bufferWidth;
    GLint bufferHeight;
    GLFWwindow *window;
    
    std::function<void(float deltaTime)> update;
    
    ImGUIModule m_imguiModule = ImGUIModule();
};
