#version 460 core
in _Vertex { vec3 Position; vec3 Normal; vec2 Texcoord; } Vertex;
out vec4 gl_Color;
uniform vec4 Tint;
uniform sampler2D Texture0;
void main() { gl_Color = Tint * texture(Texture0, Vertex.Texcoord); }