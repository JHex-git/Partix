#ifndef RANDOMROTATION_GLSL
#define RANDOMROTATION_GLSL
#include "particle_common.glsl"
#include "Math/quaternion.glsl"

void RandomRotation_Init(inout Particle particle)
{
    particle.rotation = Quaternion_RandomQuaternion(particle);
}

void RandomRotation_Update(inout Particle particle)
{
    
}
#endif // RANDOMROTATION_GLSL