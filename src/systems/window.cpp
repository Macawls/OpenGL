#include "window.h"
#include <cstdio>
#include "../utils/utils.h"

Window::Window(int width, int height, const char* title) : width(width), height(height), title(title) {
    if (!glfwInit()) {
        // init error
    }

    // window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
    }

    // window context
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }
}

Window::~Window() {
    glfwTerminate();
}

void Window::Close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void Window::Run(std::function<void()> renderLoop) {
    double lastTime = glfwGetTime();
    int frameCount = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        renderLoop();
        
        glfwSwapBuffers(window);
        
        updateTitleWithFPS(window, frameCount, lastTime, title);
        frameCount++;
    }
}

void Window::ToggleFullscreen() {
    if (isFullscreen) {
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, width, height, GLFW_DONT_CARE);
        isFullscreen = false;
    } else {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    }
}

GLFWwindow* Window::GetGLFWWindow() {
    return window;
}
