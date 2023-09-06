R"glsl(
#version 330 core
in vec3 FragColor; // Receive the color from the vertex shader

out vec4 FragOutput;

void main()
{
    FragOutput = vec4(FragColor, 1.0);
}
)glsl"