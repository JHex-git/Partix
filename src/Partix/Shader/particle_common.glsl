#ifndef PARTICLE_COMMON_GLSL
#define PARTICLE_COMMON_GLSL

struct Particle {
    bool alive;
    bool prevAlive;
    float lifetime;
    float spawnTime;
    vec3 position;
    vec4 rotation;
    vec3 velocity;
    float size;
    vec3 color;
    uvec4 seed;
    PARTICLE_EXTRA_ATTRIBUTES
};
#endif // PARTICLE_COMMON_GLSL