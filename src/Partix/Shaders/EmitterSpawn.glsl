#ifndef EMITTERSPAWN_GLSL
#define EMITTERSPAWN_GLSL
#include "common.incl"

layout(std140) uniform EmitterSpawn
{
    vec3 position;
};

EmitterSpawn_Init(EmitterSpawn module, Particle& particle)
{
    particle.alive = true;
    particle.lifetime = 10.0f;
    particle.position = module.position;
    particle.velocity = vec3(0.0f, 0.0f, 0.0f);
}

EmitterSpawn_Update(EmitterSpawn module, Particle& particle)
{
    
}
#endif // EMITTERSPAWN_GLSL