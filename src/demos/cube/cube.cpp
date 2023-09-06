#include "cube.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../../imgui/imgui.h"
#include "../../utils/logger.h"
#include "../../utils/gl_call.h"

CubeDemo::CubeDemo(WindowContext &context)
{
    window = context.GetGLFWWindow();
    camera = PerspectiveCamera(context);

    InitCube();

    gameUpdate = [&](float deltaTime)
    {
        glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), cubeScale);
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), cubeTranslation);
        glm::mat4 rotation = glm::eulerAngleXYZ(glm::radians(cubeRotation.x), glm::radians(cubeRotation.y), glm::radians(cubeRotation.z));

        glm::mat4 model = translation * rotation * scale;

        // Calculate the MVP matrix
        glm::mat4 mvp = projection * view * model;

        cubeShader.Use().SetMat4("mvp", mvp);

        // Draw cube
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Update camera
        camera.OnUpdate(deltaTime);
    };

    uiUpdate = [&]()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 370, 20));
        ImGui::SetNextWindowSize(ImVec2(350, 0));
        ImGui::Begin("Tooltips");
        ImGui::Text("Play around with use lines");
        ImGui::Text("combined with cull face modes");
        ImGui::End();
        
        
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::SetNextWindowSize(ImVec2(500, 0), ImGuiCond_Always);
        
        ImGui::Begin("Cube Demo");
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Spacing();
        ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        ImGui::Spacing();
        ImGui::Text("Frame Time: %.4f ms", ImGui::GetIO().DeltaTime * 1000.0f);

        if (ImGui::Checkbox("Use Lines", &context.renderConfig.useLines))
        {
            context.CycleRenderMode();
        }
            
        if (ImGui::Checkbox("Enable Face Culling", &cullFaces))
        {
            if (cullFaces)
            {
                glEnable(GL_CULL_FACE);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        }

        if (cullFaces)
        {
            const char *cullFaceModes[] = { "GL_BACK", "GL_FRONT", "GL_FRONT_AND_BACK" };
            ImGui::Combo("Cull Face Mode", &cullFaceMode, cullFaceModes, IM_ARRAYSIZE(cullFaceModes));

            switch (cullFaceMode)
            {
            case 0:
                glCullFace(GL_BACK);
                break;
            case 1:
                glCullFace(GL_FRONT);
                break;
            case 2:
                glCullFace(GL_FRONT_AND_BACK);
                break;
            default:
                break;
            }
        }

        if (cullFaces)
        {
            const char *frontFaceModes[] = { "GL_CCW", "GL_CW" };
            ImGui::Combo("Front Face Mode", &frontFaceMode, frontFaceModes, IM_ARRAYSIZE(frontFaceModes));
            glFrontFace(frontFaceMode == 0 ? GL_CCW : GL_CW);
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        if (ImGui::BeginTabBar("Tabs"))
        {
            if (ImGui::BeginTabItem("Parameters"))
            {
                if (ImGui::BeginTabBar("Control Tabs"))
                {
                    if (ImGui::BeginTabItem("Cube"))
                    {
                        ImGui::SliderFloat3("Translation", glm::value_ptr(cubeTranslation), -5.0f, 5.0f);
                        ImGui::SliderFloat3("Rotation", glm::value_ptr(cubeRotation), -180.0f, 180.0f);
                        ImGui::SliderFloat3("Scale", glm::value_ptr(cubeScale), 0.1f, 10.0f);

                        if (ImGui::Button("Reset"))
                        {
                            cubeTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
                            cubeRotation = glm::vec3(25.0f, 45.0f, 0.0f);
                            cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);
                        }

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Camera"))
                    {
                        ImGui::SliderFloat3("Position", glm::value_ptr(camera.position), -3.0f, 3.0f);
                        ImGui::SliderFloat3("Front", glm::value_ptr(camera.front), -3.0f, 3.0f);
                        ImGui::SliderFloat3("Up", glm::value_ptr(camera.up), -3.0f, 3.0f);
                        ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 120.0f);

                        if (ImGui::Button("Reset"))
                        {
                            camera = PerspectiveCamera(context);
                        }

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Background"))
                    {
                        ImGui::ColorEdit4("Background", glm::value_ptr(clearColour));
                        ImGui::EndTabItem();

                        if (ImGui::Button("Reset"))
                        {
                            clearColour = glm::vec4(0.18f, 0.18f, 0.18f, 1.0f);
                        }
                    }

                    ImGui::EndTabBar();
                }
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

void CubeDemo::InitCube()
{
    // VAO
    GLCall(glGenVertexArrays(1, &cubeVAO));
    GLCall(glBindVertexArray(cubeVAO));

    // VBO
    GLCall(glGenBuffers(1, &cubeVBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubeVBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors));

    // EBO
    GLCall(glGenBuffers(1, &cubeEBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    // Set up vertex attribute pointers for position and color
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)sizeof(vertices)));
    GLCall(glEnableVertexAttribArray(1));

    // Unbind VBO and VAO
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}