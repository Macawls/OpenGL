#define GLEW_STATIC // BEFORE including glew.h!
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <cstdio>
#include "utils.h"
#include "shaderutils.h"

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
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

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

    double lastTime = glfwGetTime();
    int frameCount = 0;
    float translationX = 0.0f;
    float translationY = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translationX, translationY, 0.0f));

        glUseProgram(shaderProgram);

        GLuint translationMatrixLoc = glGetUniformLocation(shaderProgram, "translation");
        glUniformMatrix4fv(translationMatrixLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            translationX -= 0.001f;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            translationX += 0.001f;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            translationY += 0.001f;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            translationY -= 0.001f;
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        updateWindowTitle(window, frameCount, lastTime, WINDOW_TITLE);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}