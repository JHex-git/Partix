#include "shader.incl"
SHADER_BEGIN
layout (location = 0) in vec3 aPos;

layout(binding = 0) uniform Testbuffer
{
    mat4 view;
    mat4 projection;
} testbuffer;


void main() {
    gl_Position = testbuffer.projection * testbuffer.view * vec4(aPos, 1.0);
}