#version 330 core

layout (location = 0) in vec2 tp_in;

uniform vec2 scale;

void main()
{
   vec2 tp = 2.0 * tp_in * scale - 1.0;
   gl_Position = vec4(tp.x, -tp.y, 0.0, 1.0);
}
