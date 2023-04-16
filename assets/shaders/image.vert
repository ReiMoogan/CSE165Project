#version 330 core

in vec4 vertex;
in vec4 texCoord;
out vec4 texc;
uniform mediump mat4 matrix;

void main(void)
{
    gl_Position = matrix * vertex;
    texc = texCoord;
}