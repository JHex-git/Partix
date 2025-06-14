#include "shader.glsl"
SHADER_BEGIN
#include "particle_common.glsl"
#include "view.glsl"

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(location = 0) flat in uint ID;
layout(location = 1) in vec2 TexCoord;
out vec4 outColor;

void main()
{
    Particle particle = particle_buffer.particles[ID];
    float change_rate = 10.f;
    vec3 color = particle.color;
    vec2 center_uv = abs(TexCoord - vec2(0.5, 0.5));
    float exponent = 10.0;
    float gradient = clamp(pow(1-(center_uv.x + center_uv.y), exponent), 0, 1) * ((sin(particle.lifetime * change_rate) + 1) * 0.25 + 0.5);
    outColor = vec4(mix(color, vec3(1.0, 1.0, 1.0), gradient), gradient);
}