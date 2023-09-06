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

    // VAO
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    // VBO
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors));

    // EBO
    GLCall(glGenBuffers(1, &EBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    // Set up vertex attribute pointers for position and color
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)sizeof(vertices)));
    GLCall(glEnableVertexAttribArray(1));

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    GLCall(glBindVertexArray(0));

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

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Update camera
        camera.OnUpdate(deltaTime);
    };

    uiUpdate = [&]()
    {
        ImGui::Begin("Cube Demo");
       

        ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Spacing();
        ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        ImGui::Spacing();
        ImGui::Text("Frame Time: %.4f ms", ImGui::GetIO().DeltaTime * 1000.0f);

        if (ImGui::BeginTabBar("Okay"))
        {
            if (ImGui::BeginTabItem("Cube"))
            {
                ImGui::SliderFloat3("Translation", glm::value_ptr(cubeTranslation), -3.0f, 3.0f);
                ImGui::Spacing();
                ImGui::SliderFloat3("Rotation", glm::value_ptr(cubeRotation), -180.0f, 180.0f);
                ImGui::Spacing();
                ImGui::SliderFloat3("Scale", glm::value_ptr(cubeScale), 0.5f, 3.0f);

                if (ImGui::Button("Reset"))
                {
                    cubeTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
                    cubeRotation = glm::vec3(0.0f, 0.0f, 0.0f);
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
                ImGui::SliderFloat("Sensitivity", &camera.sensitivity, 0.05f, 0.9f);

                
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
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    };
}
