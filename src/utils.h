#ifndef UTILS_H
#define UTILS_H

#include <GLFW/glfw3.h>

/* Update the title of a GLFW window with the current frames per second (FPS). */
void updateWindowTitle(GLFWwindow* window, int frameCount, double lastTime, const char* customTitle);

#endif // UTILS_H
