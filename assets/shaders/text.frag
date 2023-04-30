#version 330 core

in vec2 texc;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;
uniform int visible;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texc).r);
    if (sampled.a < 0.001 || visible == 0)
        discard;
    color = textColor * sampled;
}