#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 0) out vec2 TexCoord;
void main()
{
    TexCoord = inTexCoord;
    gl_Position = vec4(inPosition, 0.0, 1.0);
}