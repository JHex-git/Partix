#include <Attrix/Particle/ParticleDefines.h>

namespace Partix
{
BEGIN_PARTICLE_ATTRIBUTES(Test)
    float a;
END_PARTICLE_ATTRIBUTES()

IMPLEMENT_PARTICLE_ATTRIBUTES(Test, "float a;")
} // Partix