#define GLEW_STATIC  // BEFORE including glew.h!
#include <GL/glew.h> // always put glew.h before glfw3.h
#include <GLFW/glfw3.h>
#include <cstdio>

// local
#include "utils/logger.h"
#include "systems/window.h"
#include "systems/rendering/shaders/shader.h"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// imgui
#include "imgui/imgui_impl_glfw.h"

#include <string>
#include <cstdio>

#define WIDTH 1600
#define HEIGHT 900
#define WINDOW_TITLE "Triangle Demo"

int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);

    Window window = Window(WIDTH, HEIGHT, WINDOW_TITLE);

    const char *vertexSource =
#include "resources/shaders/TriangleDemo/shader.vert"
        ;

    const char *fragSource =
#include "resources/shaders/TriangleDemo/shader.frag"
        ;

    Shader triangleShader(vertexSource, fragSource);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    float colours[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f};

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

    GLuint colours_vbo = 0;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glfwSetKeyCallback(window.GetGLFWWindow(), [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
        Window* win = (Window*)glfwGetWindowUserPointer(window);
        if (key == GLFW_KEY_F && action == GLFW_PRESS)
        {
            win->ToggleFullscreen();
        }
        if (key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            win->CycleRenderMode();
        } });

    // Variables

    // Translation
    float translationX = 0.0f;
    float translationY = 0.0f;
    float translationXMax = 0.5f;
    float translationYMax = 0.5f;
    float translationSpeed = 2.0f;
    // Scale
    float scaleFactor = 1.0f;
    float scaleSpeed = 1.5f;
    float scaleMax = 2.0f;
    float scaleMin = 0.1f;

    // Rotation
    float rotationAngle = 0.0f;
    float rotationSpeed = 10.0f;

    ImVec4 clearColour = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    GLFWwindow *win = window.GetGLFWWindow();

    auto game = [&](float deltaTime)
    {
        glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translationX, translationY, 0.0f));
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, 1.0f));
        glm::mat4 modelMatrix = translationMatrix * scalingMatrix * rotationMatrix;

        triangleShader.Use().SetMat4("modelMatrix", modelMatrix);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        {
            translationX -= translationSpeed * deltaTime;
        }

        if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        {
            translationX += translationSpeed * deltaTime;
        }

        if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        {
            translationY += translationSpeed * deltaTime;
        }

        if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        {
            translationY -= translationSpeed * deltaTime;
        }

        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
        {
            scaleFactor -= scaleSpeed * deltaTime;
        }

        if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS)
        {
            scaleFactor += scaleSpeed * deltaTime;
        }

        if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS)
        {
            rotationAngle += rotationSpeed * deltaTime;
        }

        if (glfwGetKey(win, GLFW_KEY_T) == GLFW_PRESS)
        {
            rotationAngle -= rotationSpeed * deltaTime;
        }

        // Clamping

        translationX = glm::clamp(translationX, -translationXMax, translationXMax);
        translationY = glm::clamp(translationY, -translationYMax, translationYMax);
        scaleFactor = glm::clamp(scaleFactor, scaleMin, scaleMax);
        rotationAngle = std::fmod(rotationAngle, glm::two_pi<float>());
    };

    ImVec2 space = ImVec2(0, 10);
    const unsigned char *renderer = glGetString(GL_RENDERER);

    auto ui = [&]()
    {
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoMove);

        if (ImGui::BeginTabBar("Tabs"))
        {
            if (ImGui::BeginTabItem("Debug"))
            {
                ImGui::Text("Renderer: %s", renderer);
                ImGui::Spacing();
                ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
                ImGui::Spacing();
                ImGui::Text("Frame Time: %.4f ms", ImGui::GetIO().DeltaTime * 1000.0f);
                ImGui::Dummy(space);

                ImGui::Text("Translation");
                ImGui::Spacing();
                ImGui::SliderFloat("X", &translationX, -translationXMax, translationXMax);
                ImGui::SliderFloat("Y", &translationY, -translationYMax, translationYMax);
                ImGui::Dummy(space);

                ImGui::Text("Scale");
                ImGui::Spacing();
                ImGui::SliderFloat("Factor", &scaleFactor, scaleMin, scaleMax);
                ImGui::Dummy(space);

                ImGui::Text("Rotation");
                ImGui::Spacing();
                ImGui::SliderAngle("Angle", &rotationAngle);
                ImGui::Dummy(space);

                
                // check if any of the values are different from the default
                if (translationX != 0.0f || translationY != 0.0f || scaleFactor != 1.0f || rotationAngle != 0.0f)
                {
                    if (ImGui::Button("Reset"))
                    {
                        translationX = 0.0f;
                        translationY = 0.0f;
                        scaleFactor = 1.0f;
                        rotationAngle = 0.0f;
                    }

                    ImGui::Dummy(space);
                }
                
            
                ImGui::Text("Rendering");
                ImGui::Spacing();
                if (ImGui::Checkbox("Use Lines", &window.useLines))
                {
                    window.CycleRenderMode();
                }

                ImGui::Dummy(space);
                ImGui::ColorPicker4("background", (float *)&clearColour);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Controls"))
            {
                ImGui::Spacing();
                ImGui::Text("WASD or Arrow Keys to move");
                ImGui::Spacing();
                ImGui::Text("Q/E to scale");
                ImGui::Spacing();
                ImGui::Text("R/T to rotate");
                ImGui::Spacing();
                ImGui::Text("F to toggle fullscreen");
                ImGui::Spacing();
                ImGui::Text("P to toggle wireframe mode");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Logs"))
            {
                ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                for (const std::string &logEntry : Logger::GetHistory())
                {
                    ImGui::TextUnformatted(logEntry.c_str());
                }
                ImGui::EndChild();
                
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    };

    window.SetPrimaryUpdate(game);
    window.SetSecondaryUpdate(ui);
    window.BeginLoop();

    return 0;
}