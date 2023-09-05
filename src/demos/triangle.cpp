#define GLEW_STATIC  // BEFORE including glew.h!
#include <GL/glew.h> // always put glew.h before glfw3.h
#include <GLFW/glfw3.h>

#include "triangle.h"

#include "../imgui/imgui.h"

TriangleDemo::TriangleDemo(WindowContext &context)
{
    window = context.GetGLFWWindow();
    
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        WindowContext* win = (WindowContext*)glfwGetWindowUserPointer(window);
        
        if (key == GLFW_KEY_F && action == GLFW_PRESS) win->ToggleFullscreen();
        if (key == GLFW_KEY_P && action == GLFW_PRESS) win->CycleRenderMode();
    });

 /*
     triangleShader = Shader()
    .SetVertexSource(vertexSource)
    .SetFragmentSource(fragSource)
    .Compile();

 */

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

    gameUpdate = [&](float deltaTime)
    {
        glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translationParams.translationX, translationParams.translationY, 0.0f));
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationParams.rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleParams.scaleFactor, scaleParams.scaleFactor, 1.0f));
        glm::mat4 modelMatrix = translationMatrix * scalingMatrix * rotationMatrix;

        triangleShader.Use().SetMat4("modelMatrix", modelMatrix);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            translationParams.translationX -= translationParams.translationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            translationParams.translationX += translationParams.translationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            translationParams.translationY += translationParams.translationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            translationParams.translationY -= translationParams.translationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            scaleParams.scaleFactor -= scaleParams.scaleSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            scaleParams.scaleFactor += scaleParams.scaleSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            rotationParams.rotationAngle += rotationParams.rotationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            rotationParams.rotationAngle -= rotationParams.rotationSpeed * deltaTime;
        }

        // Clamping

        translationParams.translationX = glm::clamp(translationParams.translationX, -translationParams.translationXMax, translationParams.translationXMax);
        translationParams.translationY = glm::clamp(translationParams.translationY, -translationParams.translationYMax, translationParams.translationYMax);
        
        scaleParams.scaleFactor = glm::clamp(scaleParams.scaleFactor, scaleParams.scaleMin, scaleParams.scaleMax);
        rotationParams.rotationAngle = std::fmod(rotationParams.rotationAngle, glm::two_pi<float>());
    };


    ImVec2 space = ImVec2(0, 10);
    
    uiUpdate = [&]()
    {
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoMove);

        if (ImGui::BeginTabBar("Tabs"))
        {
            if (ImGui::BeginTabItem("Debug"))
            {
                ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
                ImGui::Spacing();
                ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
                ImGui::Spacing();
                ImGui::Text("Frame Time: %.4f ms", ImGui::GetIO().DeltaTime * 1000.0f);
                ImGui::Dummy(space);

                ImGui::Text("Translation");
                ImGui::Spacing();
                
                ImGui::SliderFloat("X", &translationParams.translationX, -translationParams.translationXMax, translationParams.translationXMax);
                ImGui::SliderFloat("Y", &translationParams.translationY, -translationParams.translationYMax, translationParams.translationYMax);
                ImGui::Dummy(space);

                ImGui::Text("Scale");
                ImGui::Spacing();
                ImGui::SliderFloat("Factor", &scaleParams.scaleFactor, scaleParams.scaleMin, scaleParams.scaleMax);
                ImGui::Dummy(space);

                ImGui::Text("Rotation");
                ImGui::Spacing();
                ImGui::SliderAngle("Angle", &rotationParams.rotationAngle);
                ImGui::Dummy(space);

                if (translationParams.translationX != 0.0f || translationParams.translationY != 0.0f || scaleParams.scaleFactor != 1.0f || rotationParams.rotationAngle != 0.0f)
                {
                    if (ImGui::Button("Reset"))
                    {
                        translationParams.translationX = 0.0f;
                        translationParams.translationY = 0.0f;
                        scaleParams.scaleFactor = 1.0f;
                        rotationParams.rotationAngle = 0.0f;
                    }

                    ImGui::Dummy(space);
                }
                
            
                ImGui::Text("Rendering");
                ImGui::Spacing();
               
                if (ImGui::Checkbox("Use Lines", &context.renderConfig.useLines))
                {
                    context.CycleRenderMode();
                }

                if (ImGui::Checkbox("Use Fullscreen", &context.renderConfig.fullscreen))
                {
                    context.ToggleFullscreen();
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


}