#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <cstdio>

#include "utils/logger.h"
#include "utils/shaderutils.h"
#include "systems/window.h"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 1280
#define HEIGHT 720
#define WINDOW_TITLE "Moving Triangle"

const GLchar *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 position;
    uniform mat4 translation;  // Add this line
    void main()
    {
        gl_Position = translation * vec4(position, 1.0);
    }
)";

const GLchar *fragmentShaderSource = R"(
    #version 330 core
    out vec4 color;
    void main()
    {
        color = vec4(0.0, 0.0, 1.0, 1.0); // Blue color
    }
)";

int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);
    Window window = Window(WIDTH, HEIGHT, WINDOW_TITLE);

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = linkShaderProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    float translationX = 0.0f;
    float translationY = 0.0f;

    GLFWwindow* win = window.GetGLFWWindow();

    glfwSetKeyCallback(win, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* win = (Window*)glfwGetWindowUserPointer(window);
        if (key == GLFW_KEY_F && action == GLFW_PRESS)
        {
            win->ToggleFullscreen();
        }
    });

    auto updateFunction = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translationX, translationY, 0.0f));

        GLuint translationMatrixLoc = glGetUniformLocation(shaderProgram, "translation");
        glUniformMatrix4fv(translationMatrixLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        {
            translationX -= 0.001f;
        }

        if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        {
            translationX += 0.001f;
        }

        if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        {
            translationY += 0.001f;
        }

        if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        {
            translationY -= 0.001f;
        }
    };

    window.Run(updateFunction);
    return 0;
}