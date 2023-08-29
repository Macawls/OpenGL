#include "window.h"
#include <cstdio>
#include "../utils/logger.h"


#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <string>


Window::Window(int width, int height, const char *title) : width(width), height(height), title(title)
{
    if (!glfwInit())
    {
        Logger::LogError("Failed to initialize GLFW");
    }

    Logger::LogDebug("Initialized GLFW");

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // MSAA
    glfwWindowHint(GLFW_SAMPLES, 2);
    glEnable(GL_MULTISAMPLE);

    // Polygon smooth (antialiasing)
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
    {
        Logger::LogError("Failed to create GLFW window");
    }
    Logger::LogDebug("Created GLFW window");

    // Get buffer size information
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set opengl context, before glew init
    glfwMakeContextCurrent(window);

    // Make sure viewport updates when window is resized
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
    { 
        glViewport(0, 0, width, height); 
    });

    // Set viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Set user pointer to this class, so we can access it in callbacks
    glfwSetWindowUserPointer(window, this);

    // Enable VSync
    //glfwSwapInterval(1);
    //useVSync = true;
    
    GLenum err = glewInit();
    
    if (err != GLEW_OK)
    {
        Logger::LogError("GLEW Init Failed, Error: %s", glewGetErrorString(err));
        glfwTerminate();
        return;
    }

    // Init ImGui with window context
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); 
    // Style
    ImGui::StyleColorsDark();

    // check if imgui init was successful
    if (ImGui::GetCurrentContext() == nullptr)
    {
        Logger::LogError("ImGui init failed");
        glfwTerminate();
        return;
    }
    else {
        Logger::LogDebug("ImGui init success, version: %s", IMGUI_VERSION);
    }

    Logger::LogDebug("OpenGL Version: %s", glGetString(GL_VERSION));
    Logger::LogDebug("OpenGL Vendor: %s", glGetString(GL_VENDOR));
    Logger::LogDebug("OpenGL Renderer: %s", glGetString(GL_RENDERER));
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::BeginLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        primary(ImGui::GetIO().DeltaTime);
        secondary();
        
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }
}