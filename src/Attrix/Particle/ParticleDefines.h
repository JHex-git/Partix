#include <Attrix/ParticleFactory.h>

#define BEGIN_PARTICLE_ATTRIBUTES(type_name) \
    namespace \
    { \
        struct alignas(16) type_name \
        {
#define END_PARTICLE_ATTRIBUTES() \
        };   \
    }

#define IMPLEMENT_PARTICLE_ATTRIBUTES(type_name, fields) \
    namespace \
    { \
        struct type_name##ParticleRegister \
        { \
            type_name##ParticleRegister() \
            { \
                ParticleFactory::Instance().RegisterParticle<type_name>(#type_name, fields); \
            } \
        } type_name##_particle_register; \
    }
