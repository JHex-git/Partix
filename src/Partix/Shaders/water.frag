#include "shader.glsl"
SHADER_BEGIN

struct Particle {
    bool alive;
    bool prevAlive;
    float lifetime;
    vec3 position;
    vec3 velocity;
    float size;
    vec3 color;
    uvec4 seed;
};

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(location = 0) in vec2 TexCoord;
layout(location = 1) flat in uint ID;
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