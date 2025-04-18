#version 450

layout(location = 0) in vec2 TexCoord;
layout(binding = 0) uniform sampler2D utexture;
out vec4 outColor;

bool CircleMask(vec2 uv, vec2 center, float radius)
{
    return length(uv - center) < radius;
}

void main()
{
    if (CircleMask(TexCoord, vec2(0.5, 0.5), 0.5))
    {
        outColor = texture(utexture, TexCoord);
    }
    else
    {
        outColor = vec4(1.0, 0.0, 0.0, 0.0); // Transparent color
    }
}