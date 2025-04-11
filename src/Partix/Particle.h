#pragma once
#include <glm/glm.hpp>

namespace Partix
{

struct alignas(16) Particle
{
    bool alive;
    float lifecycle;
    vec3 position;
    vec3 velocity;
};
} // namespace Partix
