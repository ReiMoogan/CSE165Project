#version 330 core

in vec4 texc;
uniform sampler2D text;
out vec4 color;

void main(void)
{
    vec4 texColor = texture(text, texc.st);
    if (texColor.a < 0.001) {
        discard;
    }
    color = texColor;
}