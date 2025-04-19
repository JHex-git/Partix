#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Partix/Component.h>
#include <Partix/ShaderProgram.h>
namespace Partix
{

struct DefaultAttributes{};

template <typename ExtraAttrib = DefaultAttributes>
struct alignas(16) Emitter
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    int emitCountPerFrame;
    float lifetime;
    float emitVelocity;
    float jitterAngleRange;
    float spriteSize = 0.1f;
    int maxParticleCount;
    ExtraAttrib attributes;
};

// Empty struct will still take one byte, so specialize for DefaultAttributes to avoid that.
template <>
struct alignas(16)  Emitter<DefaultAttributes>
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    int emitCountPerFrame;
    float lifetime;
    float emitVelocity;
    float jitterAngleRange;
    float spriteSize = 0.1f;
    int maxParticleCount;
};
} // namespace Partix
