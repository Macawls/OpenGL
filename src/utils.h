#include <stdio.h>

/* Update the title of a GLFW window with the current frames per second (FPS). */
static void updateWindowTitle(GLFWwindow* window, int frameCount, double lastTime, const char* customTitle) {
    double currentTime = glfwGetTime();
    if (currentTime - lastTime >= 1.0) {
        double fps = (double)frameCount / (currentTime - lastTime);
        char title[256];
        snprintf(title, sizeof(title), "%s - %.2f FPS", customTitle, fps);
        glfwSetWindowTitle(window, title);
    }
}