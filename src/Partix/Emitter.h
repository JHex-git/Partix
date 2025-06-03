#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <Partix/Component.h>
#include <Partix/ShaderProgram.h>
#include <Partix/Attribute.h>
#include <Partix/Mesh.h>

namespace Partix
{

struct EmitterBase{};

template <typename ExtraAttrib = DefaultAttributes>
struct alignas(16) Emitter final : EmitterBase
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    int emitCountPerFrame;
    float lifetime;
    float emitVelocity;
    float spriteSize = 0.1f;
    int maxParticleCount;
    ExtraAttrib attributes;
};

// Empty struct will still take one byte, so specialize for DefaultAttributes to avoid that.
template <>
struct alignas(16)  Emitter<DefaultAttributes> final : EmitterBase
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    int emitCountPerFrame;
    float lifetime;
    float emitVelocity;
    float spriteSize = 0.1f;
    int maxParticleCount;
};

struct EmitterShaderInfo
{
    std::string simulate_shader_path;
    std::string sprite_shader_path;
    std::shared_ptr<Mesh> mesh;
    std::vector<std::string> sprite_shader_texture_paths;
    std::vector<int> sprite_texture_bindings;
    std::map<std::string, std::string> defines;

    static const std::string ParticleExtraAttributes;
    EmitterShaderInfo()
    {
        defines[ParticleExtraAttributes] = "";
    }
};
} // namespace Partix
