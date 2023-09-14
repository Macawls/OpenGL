#pragma once

#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../utils/logger.h"
#include "imgui_module.h"

#include <functional>

struct WindowStatus
{
    bool isFullscreen = false;
    bool isMinimized = false;
    bool isVSync = false;
};

struct WindowParams
{
    int width;
    int height;
    const char *title;
    float imguiFontSize;
    GLFWimage iconImage;
};

struct FramebufferSize
{
    int width;
    int height;
};

// A Wrapper for GLFWwindow, also manages ImGui and OpenGL context
class WindowContext
{
public:
    WindowContext(WindowParams params);
    ~WindowContext();
    ImGUIModule ImGUI = ImGUIModule();
    
    // Read only
    WindowStatus WindowStatus;
    
    // Sets the update function, called every frame
    void SetDeltaUpdate(std::function<void(float deltaTime)> func) { this->m_update = func; }
    
    // Closes the window
    void Close();
    // Begins the main loop, update function is called every frame
    void BeginLoop();

    // Toggle's VSync, static because there's only one window really
    static void ToggleVSync(bool enable) { glfwSwapInterval(enable ? 1 : 0); }

    // Returns the GLFWwindow pointer
    GLFWwindow *GetGLFWWindow() const { return m_window; }
    
    // Returns the video mode of the primary monitor
    static GLFWvidmode GetVideoMode() { return *glfwGetVideoMode(glfwGetPrimaryMonitor()); }
    
    // Returns the framebuffer size
    FramebufferSize* GetFrameBufferSize() { return &m_FramebufferSize; }
    
    // Returns the framebuffer aspect ratio
    float GetFrameBufferAspectRatio() { return (float)m_FramebufferSize.width / (float)m_FramebufferSize.height; }

private:
    GLFWwindow *m_window;
    std::function<void(float deltaTime)> m_update;
    FramebufferSize m_FramebufferSize = { 0, 0 };
    
    bool Init(WindowParams params);
    void SetStandardCallbacks();
    void SetStandardWindowHints();
};
