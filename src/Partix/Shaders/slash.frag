#include "shader.glsl"
SHADER_BEGIN
#include "particle_common.glsl"
#include "view.glsl"

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(location = 0) flat in uint ID;
layout(location = 1) in vec2 TexCoord;
layout(binding = 0) uniform sampler2D uTexture;
out vec4 outColor;

void main()
{
    float color = pow(1 - abs(TexCoord.x - 0.5) * 2, 5) * pow(1 - abs(TexCoord.y + mix(-1, 1, (sin(view.currentTime) + 1) * 0.5) - 0.5) * 2, 5);
    outColor = vec4(color.xxx * texture(uTexture, TexCoord).rgb, color.x);
}