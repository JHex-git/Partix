#ifndef EMITTERSPAWN_GLSL
#define EMITTERSPAWN_GLSL
#include "particle_common.glsl"

void EmitterSpawn_Init(inout Particle particle)
{
    particle.prevAlive = particle.alive;
    if (!particle.alive)
    {
        // Check if we can emit a new particle
        uint count = atomicCounterIncrement(emitCount);
        if (count < emitter.emitCountPerFrame)
        {
            particle.alive = true;
            particle.lifetime = emitter.lifetime + (RandomFloat(particle) - 0.5) * emitter.lifetime * 0.25; // Random lifetime
            particle.position = vec3(emitter.position); // Random position
            particle.size = emitter.spriteSize;
        }
    }
    else
    {
        particle.lifetime -= view.deltaTime;
        if (particle.lifetime <= 0.0)
        {
            particle.alive = false;
        }
    }
}

void EmitterSpawn_Update(inout Particle particle)
{
    
}
#endif // EMITTERSPAWN_GLSL