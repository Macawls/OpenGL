R"glsl(
#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 modelMatrix;
out vec3 colour;

void main() 
{
    colour = vertex_colour;
    gl_Position = modelMatrix * vec4(vertex_position, 1.0);
}
)glsl"