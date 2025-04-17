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
    int emitCountPerFrame;
    glm::vec3 position;
    glm::vec3 direction;
    float lifetime;
};
} // namespace Partix
