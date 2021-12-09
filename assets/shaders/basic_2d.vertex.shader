#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out _Vertex { vec3 Position; vec3 Normal; vec2 Texcoord; } Vertex;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main() {
    Vertex.Position = a_Position;
    Vertex.Normal = a_Normal;
    Vertex.Texcoord = a_Texcoord;
    gl_Position = (Projection * View * Model) * vec4(Vertex.Position, 1.0);
}