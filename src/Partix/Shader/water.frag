#include "shader.glsl"
SHADER_BEGIN
#include "particle_common.glsl"

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(location = 0) flat in uint ID;
layout(location = 1) in vec2 TexCoord;
layout(binding = 0) uniform sampler2D utexture;
out vec4 outColor;

bool CircleMask(vec2 uv, vec2 center, float radius)
{
    return length(uv - center) < radius;
}

void main()
{
    Particle particle = particle_buffer.particles[ID];
    if (CircleMask(TexCoord, vec2(0.5, 0.5), 0.5))
    {
        // vec3 color = mix(texture(utexture, TexCoord).xyz, vec3(1, 1, 1), 1 - clamp(pow(length(TexCoord - 0.5), 2), 0, 1));
        float opacity = pow(1 - clamp(pow(length(TexCoord - 0.5) * 2, 2), 0, 1), 4);
        vec3 color = texture(utexture, TexCoord).xyz * particle.color;
        outColor = vec4(color * particle.color, opacity);
        // outColor = vec4(color, opacity);
    }
    else
    {
        outColor = vec4(0.0, 0.0, 0.0, 0.0); // Transparent color
    }
}