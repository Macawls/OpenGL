#include <stdio.h>

/* Update the title of a GLFW window with the current frames per second (FPS). */
static void updateTitleWithFPS(GLFWwindow* window, const char* customTitle) {
    static int frameCount = 0;
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    frameCount++;

    if (currentTime - lastTime >= 1.0) {
        double fps = (double)frameCount / (currentTime - lastTime);
        char title[256];
        snprintf(title, sizeof(title), "%s - %.2f FPS", customTitle, fps);
        glfwSetWindowTitle(window, title);

        lastTime = currentTime;
        frameCount = 0;  // Reset frame count for the next second
    }
}