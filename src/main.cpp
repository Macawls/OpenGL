#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h> // Include GLEW header

#include <GLFW/glfw3.h>
#include <cstdio>
#include "utils.h"

#define WIDTH 700
#define HEIGHT 480
#define WINDOW_TITLE "Open GL"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // make openGL context current before initializing GLEW
    glfwMakeContextCurrent(window); 

    // Initialize GLEW after creating the GLFW window
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    int frameCount = 0;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        updateWindowTitle(window, frameCount, lastTime, WINDOW_TITLE);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
