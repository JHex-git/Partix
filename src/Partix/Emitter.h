#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Partix/Component.h>
#include <Partix/ShaderProgram.h>
namespace Partix
{

struct alignas(16) Emitter
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    int emitCountPerFrame;
    float lifetime;
    float emitVelocity;
    float jitterAngleRange;
    float spriteSize = 0.1f;
};
} // namespace Partix
