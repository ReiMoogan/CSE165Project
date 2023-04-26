#version 330 core

in vec4 texc;
uniform sampler2D text;
uniform bool recolor;
uniform vec3 hslShift;
out vec4 color;

vec3 rgb2hsl(vec3 c)
{
    float maxVal = max(c.r, max(c.g, c.b));
    float minVal = min(c.r, min(c.g, c.b));
    float l = (maxVal + minVal) / 2.0;
    float h, s;

    if (maxVal == minVal)
    {
        h = 0;
        s = 0;
    }
    else
    {
        float d = maxVal - minVal;
        s = l > 0.5 ? d / (2.0 - maxVal - minVal) : d / (maxVal + minVal);
        if (maxVal == c.r)
        {
            h = (c.g - c.b) / d + (c.g < c.b ? 6.0 : 0.0);
        }
        else if (maxVal == c.g)
        {
            h = (c.b - c.r) / d + 2.0;
        }
        else
        {
            h = (c.r - c.g) / d + 4.0;
        }

        h /= 6.0;
    }

    return vec3(h, s, l);
}

float hue2rgb(vec3 c) {
    if (c.z < 0.0) {
        c.z += 1.0;
    }
    if (c.z > 1.0) {
        c.z -= 1.0;
    }
    if (c.z < 1.0 / 6.0) {
        return c.x + (c.y - c.x) * 6.0 * c.z;
    } else if (c.z < 1.0 / 2.0) {
        return c.y;
    } else if (c.z < 2.0 / 3.0) {
        return c.x + (c.y - c.x) * (2.0 / 3.0 - c.z) * 6.0;
    }
    return c.x;
}

vec3 hsl2rgb(vec3 c)
{
    // professional wikipedia reader here because color theory is hard
    // GLSL fun
    float r, g, b;

    if (c.y == 0.0)
    {
        r = g = b = c.z;
    }
    else
    {
        float q = c.z < 0.5 ? c.z * (1.0 + c.y) : c.z + c.y - c.z * c.y;
        float p = 2.0 * c.z - q;
        r = hue2rgb(vec3(p, q, c.x + 1.0 / 3.0));
        g = hue2rgb(vec3(p, q, c.x));
        b = hue2rgb(vec3(p, q, c.x - 1.0 / 3.0));
    }

    return vec3(r, g, b);
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void)
{
    vec4 texColor = texture(text, texc.st);
    if (texColor.a < 0.001) {
        discard;
    }
    if (recolor) {
        vec3 hsv = rgb2hsv(texColor.rgb);

        texColor.rgb = hsv2rgb(hsv);

        vec3 hsl = rgb2hsl(texColor.rgb);
        hsl.x = mod(hsl.x + hslShift.x, 1.0);
        hsl.y = clamp(hsl.y + hslShift.y, 0.0, 1.0);
        hsl.z = clamp(hsl.z + hslShift.z, 0.0, 1.0);
        texColor.rgb = hsl2rgb(hsl);
    }

    color = texColor;
}