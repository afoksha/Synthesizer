#version 330 core

layout (location = 0) in float sample_in;

uniform vec2 scale;
uniform vec2 shift;

void main()
{
   vec2 point = shift + scale * vec2(gl_VertexID, sample_in);
   gl_Position = vec4(point, 0.0, 1.0);
}
