#pragma once
#include <glm/glm.hpp>

namespace Partix
{

struct alignas(16) Particle
{
    bool alive = false;
    float lifetime = 0;
    glm::vec3 position = glm::vec3(0);
    glm::vec3 velocity = glm::vec3(0);
};
} // namespace Partix
