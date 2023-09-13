#define GLEW_STATIC
#include <GL/glew.h>

#include "render_settings.h"
#include "../../utils/gl_call.h"
#include <GLFW/glfw3.h>

OpenGLRenderSettings::OpenGLRenderSettings(const RenderConfig &settings)
{
    m_io = &ImGui::GetIO();
    this->m_config = settings;
}

OpenGLRenderSettings::OpenGLRenderSettings()
{
    m_io = &ImGui::GetIO();
    this->m_config = RenderConfig();
}

OpenGLRenderSettings &OpenGLRenderSettings::PolygonMode(PolygonModeConfig config)
{
    m_config.polygonMode = config;
    return *this;
}

OpenGLRenderSettings &OpenGLRenderSettings::PolygonSmoothing(PolygonSmoothingConfig config)
{
    m_config.polygonSmoothing = config;
    return *this;
}

OpenGLRenderSettings &OpenGLRenderSettings::Depth(DepthConfig config)
{
    m_config.depth = config;
    return *this;
}

OpenGLRenderSettings &OpenGLRenderSettings::Blending(BlendingConfig config)
{
    m_config.blending = config;
    return *this;
}

OpenGLRenderSettings &OpenGLRenderSettings::Culling(CullingConfig config)
{
    m_config.culling = config;
    return *this;
}

OpenGLRenderSettings &OpenGLRenderSettings::Apply()
{
    GLCall(glPolygonMode(m_config.polygonMode.face, m_config.polygonMode.mode));

    if (m_config.culling.enabled)
    {
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(m_config.culling.face));
        GLCall(glFrontFace(m_config.culling.windingOrder));
    }
    else
    {
        GLCall(glDisable(GL_CULL_FACE));
    }

    if (m_config.polygonSmoothing.enabled)
    {
        GLCall(glEnable(GL_POLYGON_SMOOTH));
        GLCall(glHint(GL_POLYGON_SMOOTH_HINT, m_config.polygonSmoothing.hint));
    }
    else
    {
        GLCall(glDisable(GL_POLYGON_SMOOTH));
    }

    if (m_config.depth.enabled)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(m_config.depth.function));
    }
    else
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    if (m_config.blending.enabled)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(m_config.blending.sourceFactor, m_config.blending.destinationFactor));
        GLCall(glBlendEquation(m_config.blending.equation));
    }
    else
    {
        GLCall(glDisable(GL_BLEND));
    }

    return *this;
}

void OpenGLRenderSettings::ShowPolygonModeControls()
{
    ImGui::Text("Mode");
    if (ImGui::RadioButton("Fill##PolygonFill", m_config.polygonMode.mode == GL_FILL))
    {
        m_config.polygonMode.mode = GL_FILL;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Line##PolygonLine", m_config.polygonMode.mode == GL_LINE))
    {
        m_config.polygonMode.mode = GL_LINE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Point##PolygonPoint", m_config.polygonMode.mode == GL_POINT))
    {
        m_config.polygonMode.mode = GL_POINT;
    }

    ImGui::Separator();

    ImGui::Text("Face");
    if (ImGui::RadioButton("Front##PolygonFront", m_config.polygonMode.face == GL_FRONT))
    {
        m_config.polygonMode.face = GL_FRONT;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Back##PolygonBack", m_config.polygonMode.face == GL_BACK))
    {
        m_config.polygonMode.face = GL_BACK;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Front and Back##PolygonFrontAndBack", m_config.polygonMode.face == GL_FRONT_AND_BACK))
    {
        m_config.polygonMode.face = GL_FRONT_AND_BACK;
    }

    ImGui::Separator();

    ImGui::Text("Smoothing");

    ImGui::Checkbox("Enabled##PolygonSmoothingEnabled", &m_config.polygonSmoothing.enabled);
    if (m_config.polygonSmoothing.enabled)
    {
        if (ImGui::RadioButton("Fastest##PolygonSmoothingFastest", m_config.polygonSmoothing.hint == GL_FASTEST))
        {
            m_config.polygonSmoothing.hint = GL_FASTEST;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Nicest##PolygonSmoothingNicest", m_config.polygonSmoothing.hint == GL_NICEST))
        {
            m_config.polygonSmoothing.hint = GL_NICEST;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Dont Care##PolygonSmoothingDontCare", m_config.polygonSmoothing.hint == GL_DONT_CARE))
        {
            m_config.polygonSmoothing.hint = GL_DONT_CARE;
        }
    }
}

void OpenGLRenderSettings::ShowDepthControls()
{
    ImGui::Checkbox("Enabled##DepthEnabled", &m_config.depth.enabled);
    if (m_config.depth.enabled)
    {
        if (ImGui::RadioButton("Never##DepthNever", m_config.depth.function == GL_NEVER))
        {
            m_config.depth.function = GL_NEVER;
        }
        //ImGui::SameLine();
        if (ImGui::RadioButton("Less##DepthLess", m_config.depth.function == GL_LESS))
        {
            m_config.depth.function = GL_LESS;
        }
        //ImGui::SameLine();
        if (ImGui::RadioButton("Equal##DepthEqual", m_config.depth.function == GL_EQUAL))
        {
            m_config.depth.function = GL_EQUAL;
        }
        //ImGui::SameLine();

        //ImGui::NewLine();

        if (ImGui::RadioButton("Less/Equal##DepthLessOrEqual", m_config.depth.function == GL_LEQUAL))
        {
            m_config.depth.function = GL_LEQUAL;
        }
        //ImGui::SameLine();
        if (ImGui::RadioButton("Greater##DepthGreater", m_config.depth.function == GL_GREATER))
        {
            m_config.depth.function = GL_GREATER;
        }
        //ImGui::SameLine();
        if (ImGui::RadioButton("Not Equal##DepthNotEqual", m_config.depth.function == GL_NOTEQUAL))
        {
            m_config.depth.function = GL_NOTEQUAL;
        }
        //ImGui::SameLine();

        //ImGui::NewLine();

        if (ImGui::RadioButton("Greater or Equal##DepthGreaterOrEqual", m_config.depth.function == GL_GEQUAL))
        {
            m_config.depth.function = GL_GEQUAL;
        }
        //ImGui::SameLine();
        if (ImGui::RadioButton("Always##DepthAlways", m_config.depth.function == GL_ALWAYS))
        {
            m_config.depth.function = GL_ALWAYS;
        }
        //ImGui::SameLine();

        //ImGui::NewLine();
    }
}

void OpenGLRenderSettings::ShowBlendingControls()
{
    ImGui::Checkbox("Enabled##BlendingEnabled", &m_config.blending.enabled);
    if (m_config.blending.enabled)
    {
        ImGui::Text("Source Factor");
        if (ImGui::RadioButton("GL_SRC_ALPHA", m_config.blending.sourceFactor == GL_SRC_ALPHA))
        {
            m_config.blending.sourceFactor = GL_SRC_ALPHA;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_ONE_MINUS_SRC_ALPHA##src_factor", m_config.blending.sourceFactor == GL_ONE_MINUS_SRC_ALPHA))
        {
            m_config.blending.sourceFactor = GL_ONE_MINUS_SRC_ALPHA;
        }

        ImGui::Text("Destination Factor");
        if (ImGui::RadioButton("GL_ONE", m_config.blending.destinationFactor == GL_ONE))
        {
            m_config.blending.destinationFactor = GL_ONE;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_ONE_MINUS_SRC_ALPHA##dest_factor", m_config.blending.destinationFactor == GL_ONE_MINUS_SRC_ALPHA))
        {
            m_config.blending.destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
        }

        ImGui::Text("Blending Equation");
        if (ImGui::RadioButton("GL_FUNC_ADD", m_config.blending.equation == GL_FUNC_ADD))
        {
            m_config.blending.equation = GL_FUNC_ADD;
        }
        if (ImGui::RadioButton("GL_FUNC_SUBTRACT", m_config.blending.equation == GL_FUNC_SUBTRACT))
        {
            m_config.blending.equation = GL_FUNC_SUBTRACT;
        }
        if (ImGui::RadioButton("GL_FUNC_REVERSE_SUBTRACT", m_config.blending.equation == GL_FUNC_REVERSE_SUBTRACT))
        {
            m_config.blending.equation = GL_FUNC_REVERSE_SUBTRACT;
        }
    }
}

void OpenGLRenderSettings::ShowCullingControls()
{
    ImGui::Checkbox("Enabled##CullingEnabled", &m_config.culling.enabled);
    if (m_config.culling.enabled)
    {
        ImGui::Text("Culling Face");
        if (ImGui::RadioButton("GL_FRONT", m_config.culling.face == GL_FRONT))
        {
            m_config.culling.face = GL_FRONT;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_BACK", m_config.culling.face == GL_BACK))
        {
            m_config.culling.face = GL_BACK;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_FRONT_AND_BACK", m_config.culling.face == GL_FRONT_AND_BACK))
        {
            m_config.culling.face = GL_FRONT_AND_BACK;
        }

        ImGui::Text("Winding Order");
        if (ImGui::RadioButton("GL_CCW", m_config.culling.windingOrder == GL_CCW))
        {
            m_config.culling.windingOrder = GL_CCW;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("GL_CW", m_config.culling.windingOrder == GL_CW))
        {
            m_config.culling.windingOrder = GL_CW;
        }
    }
}

void OpenGLRenderSettings::ShowImGuiTabBar()
{
    static RenderConfig defaultConfig = RenderConfig();
    // Note on imgui, syntax means {display}##{internal id}

    if (ImGui::BeginTabBar("Render Settings"))
    {
        if (ImGui::BeginTabItem("Polygon"))
        {
            ShowPolygonModeControls();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Depth"))
        {
            ShowDepthControls();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Blending"))
        {
            ShowBlendingControls();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Culling"))
        {
            ShowCullingControls();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Dummy(ImVec2(0.0f, 15.0f));
    if (ImGui::Button("Apply"))
    {
        Apply();
    }

    if (m_config != defaultConfig)
    {
        if (ImGui::Button("Reset"))
        {
            m_config = RenderConfig();
            Apply();
        }
    }
}

void OpenGLRenderSettings::ShowInfo(double updateRate)
{
    static double lastUpdateTime = 0.0;
    double currentTime = glfwGetTime();
    static float lastFrameTime = 0.0f;
    static float lastFPS = 0.0f;

    if (currentTime - lastUpdateTime >= updateRate)
    {
        lastFPS = m_io->Framerate;
        lastFrameTime = 1000.0f / m_io->Framerate;
        lastUpdateTime = currentTime;
    }


    ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
    ImGui::Spacing();
    ImGui::Text("FPS: %.2f", lastFPS);
    ImGui::Spacing();
    ImGui::Text("Frame Time: %.4fms", lastFrameTime);
    ImGui::Spacing();
    ImGui::Text("Window: %.0fpx x %.0fpx ", m_io->DisplaySize.x, m_io->DisplaySize.y);
}
