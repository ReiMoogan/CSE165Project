#version 330 core

in vec4 texc;
uniform sampler2D texture;
out vec4 color;

void main(void)
{
    vec4 texColor = texture2D(texture, texc.st);
    if (texColor.a < 0.001) {
        discard;
    }
    color = texColor;
}