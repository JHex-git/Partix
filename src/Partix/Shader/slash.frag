#include "shader.glsl"
SHADER_BEGIN
#include "particle_common.glsl"
#include "view.glsl"
#include "constants.glsl"

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(location = 0) flat in uint ID;
layout(location = 1) in vec2 TexCoord;
layout(binding = 0) uniform sampler2D uTexture;
out vec4 outColor;

void main()
{
    Particle particle = particle_buffer.particles[ID];
    float time = (view.currentTime - particle.spawnTime - 0.1 * PI);
    float color = pow(1 - abs(TexCoord.x - 0.5) * 2, 5) * pow(1 - abs(TexCoord.y - 0.5 + mix(-1, 1, (sin(time) + 1) * 0.5)) * 2, 5);
    outColor = vec4(color.xxx * texture(uTexture, TexCoord + vec2(particle.offset, 0)).rgb * 10, clamp(color.x, 0, 1));
}