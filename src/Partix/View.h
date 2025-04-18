#pragma once
#include <glm/glm.hpp>
namespace Partix
{

struct alignas(16) View
{
    glm::mat4 view_mat;
    glm::mat4 projection_mat;
    float prevTime;
    float currentTime;
    float deltaTime;
};
} // namespace Partix
