#pragma once
#include <glm/glm.hpp>
#include <random>
#include <Partix/Attribute.h>

// TODO: Particle Type Erasure
// No function ptr to constructor
// 1: Allocate particle memory and initialize seed afterwards
// How to preserve default value?
// particle vector initialization
// randomize seed
// 2: Register one particle for each ExtraAttrib, copy that to vector and do seed initialization afterwards
// 3: Copy Particle<DefaultAttributes> to vector, do seed initialization, register each ExtraAttrib, copy that to vector
namespace Partix
{
template <typename ExtraAttrib = DefaultAttributes>
struct alignas(16) Particle
{
    bool alive = false;
    bool prevAlive = false;
    float lifetime = 0;
    alignas(16) glm::vec3 position = glm::vec3(0);
    alignas(16) glm::vec3 velocity = glm::vec3(0);
    float size = 0.1f;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec<4, unsigned int> seed;
    ExtraAttrib attributes;

    Particle()
    {
        // Initialize seed
        static std::random_device random_device;
        static std::mt19937 engine{random_device()};
        std::uniform_int_distribution<unsigned int> seed_distribution(0, 10000);

        for (int i = 0; i < 4; ++i)
        {
            seed[i] = seed_distribution(engine);
        }
    }
};

template <>
struct alignas(16) Particle<DefaultAttributes>
{
    bool alive = false;
    bool prevAlive = false;
    float lifetime = 0;
    alignas(16) glm::vec3 position = glm::vec3(0);
    alignas(16) glm::vec3 velocity = glm::vec3(0);
    float size = 0.1f;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec<4, unsigned int> seed;
};
} // namespace Partix
