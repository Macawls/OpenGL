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
#include "imgui/imgui_impl_opengl3.h"

#define WIDTH 1600
#define HEIGHT 900
#define WINDOW_TITLE "Moving Triangle"

int main(void)
{
    Logger::SetPriority(Logger::LogPriority::Debug);

    Window window = Window(WIDTH, HEIGHT, WINDOW_TITLE);
   

    const char* vertexSource = R"glsl(
        #version 330 core
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_colour;

        uniform mat4 translation;
        out vec3 colour;

        void main() 
        {
            colour = vertex_colour;
            gl_Position = translation * vec4(vertex_position, 1.0);
        }
    )glsl";

    const char* fragSource = R"glsl(
        #version 330 core
        in vec3 colour;
        out vec4 frag_colour;

        void main() 
        {
            frag_colour = vec4(colour, 1.0);
        }
    )glsl";

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
        }
    });

    // Variables
    float translationX = 0.0f;
    float translationY = 0.0f;
    ImVec4 clearColour = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    GLFWwindow *win = window.GetGLFWWindow();
    
    auto game = [&](float deltaTime)
    {
        glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(translationX, translationY, 0.0f));
        triangleShader.Use().SetMat4("translation", matrix);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        float translationSpeed = 2.0f;

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
        
    };

    ImVec2 space = ImVec2(0, 10);
    const unsigned char* renderer = glGetString(GL_RENDERER);
    auto ui = [&]()
    {
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::Begin("Config", nullptr, ImGuiWindowFlags_NoMove);

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
                ImGui::SliderFloat("X", &translationX, -0.5f, 0.5f);
                ImGui::SliderFloat("Y", &translationY, -0.5f, 0.5f);
                ImGui::Dummy(space);

                ImGui::Text("Rendering");
                ImGui::Spacing();
                if (ImGui::Checkbox("Use Lines", &window.useLines))
                {
                    window.CycleRenderMode();
                }

                ImGui::Dummy(space);
                ImGui::ColorPicker4("background color", (float*)&clearColour);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Controls"))
            {
                ImGui::Text("WASD or Arrow Keys to move the triangle");
                ImGui::Spacing();
                ImGui::Text("F to toggle fullscreen");
                ImGui::Spacing();
                ImGui::Text("P to toggle wireframe mode");
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