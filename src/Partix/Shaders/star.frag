#version 450

layout(std140, binding = 0) uniform View
{
    mat4 view_mat;
    mat4 projection_mat;
    float prevTime;
    float currentTime;
    float deltaTime;
} view;

struct Particle {
    bool alive;
    bool prevAlive;
    float lifetime;
    vec3 position;
    vec3 velocity;
    float size;
    uvec4 seed;
};

layout(std140, binding = 1) buffer Particles {
    Particle particles[];
} particle_buffer;

layout(location = 0) in vec2 TexCoord;
layout(location = 1) flat in uint ID;
out vec4 outColor;

bool CircleMask(vec2 uv, vec2 center, float radius)
{
    return length(uv - center) < radius;
}

void main()
{
    Particle particle = particle_buffer.particles[ID];
    float change_rate = 10.f;
    vec2 star_uv = fract(TexCoord + vec2(0.5, 0.5));
    float opacity = 1.0 - float(CircleMask(star_uv, vec2(0.5, 0.5), 0.5));
    vec3 color = vec3(1, 0, 0) * ((sin(particle.lifetime * change_rate) + 1) * 0.25 + 0.5) ;
    outColor = vec4(color, opacity); // White color for the star
}