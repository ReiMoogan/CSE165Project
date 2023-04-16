#version 330 core

attribute highp vec4 vertex; // <x, y> position, <s, t> texture coordinate
out vec2 texc;
uniform mediump mat4 matrix;

void main(void)
{
    gl_Position = matrix * vec4(vertex.xy, 0.0, 1.0);
    texc = vertex.zw;
}