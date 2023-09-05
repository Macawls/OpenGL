#include "window_context.h"
#include <cstdio>
#include "../../utils/logger.h"

#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_opengl3.h"

WindowContext::WindowContext(int width, int height, const char *title, float imguiFontSize)
{
    if (!glfwInit())
    {
        Logger::LogError("Failed to initialize GLFW");
        return;
    }

    Logger::LogDebug("Initialized GLFW");

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Polygon smooth (antialiasing)
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Depth testing
    glEnable(GL_DEPTH_TEST);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
    {
        Logger::LogError("Failed to create GLFW window");
    }
    Logger::LogDebug("Created GLFW window");

    // Get buffer size information
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set opengl context, before glew and imgui init
    glfwMakeContextCurrent(window);

    // Make sure viewport updates when window is resized
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });

    // Set viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Set user pointer to this class, so we can access it in callbacks
    glfwSetWindowUserPointer(window, this);

    // Enable VSync
    // glfwSwapInterval(1);
    // useVSync = true;

    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        Logger::LogError("GLEW Init Failed, Error: %s", glewGetErrorString(err));
        glfwTerminate();
        return;
    }

    // Init ImGui with window context
    bool success = m_imguiModule.Init("#version 330", window, imguiFontSize);

    if (!success)
    {
        Logger::LogError("ImGui init failed");
        glfwTerminate();
        return;
    }

    Logger::LogDebug("ImGui init success, version: %s", IMGUI_VERSION);
    Logger::LogDebug("OpenGL Version: %s", glGetString(GL_VERSION));
    Logger::LogDebug("OpenGL Vendor: %s", glGetString(GL_VENDOR));
    Logger::LogDebug("OpenGL Renderer: %s", glGetString(GL_RENDERER));
    Logger::LogDebug("WindowContext init success");
}

WindowContext::~WindowContext()
{
    m_imguiModule.~ImGUIModule(); // destroy imgui context

    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowContext::BeginLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        m_imguiModule.BeginFrame();
        update(ImGui::GetIO().DeltaTime);
        m_imguiModule.EndFrame();

        glfwSwapBuffers(window);
    }
}

void WindowContext::CycleRenderMode()
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

void WindowContext::ToggleFullscreen()
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

void WindowContext::Close()
{
    glfwSetWindowShouldClose(window, GL_TRUE);
}