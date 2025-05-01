#pragma once
#include <vector>
#include <glad/glad.h>
#include <Partix/Texture.h>
#include <Partix/ShaderProgram.h>
#include <Partix/Emitter.h>
#include <Partix/Particle.h>

namespace Partix
{

struct View;

class PartixEngine
{
public:
    PartixEngine();
    
    void Tick(const View &view);

    void AddEmitter(const EmitterBase *emitter, int emitter_size, const EmitterShaderInfo& emitter_shader_info, const void *particles, int particle_count, int particle_size);

    template <typename ExtraAttrib, typename ParticleAttrib = DefaultAttributes>
    void AddEmitter(const Emitter<ExtraAttrib> &emitter, const EmitterShaderInfo& emitter_shader_info)
    {
        EmitterContext context;

        const Emitter<DefaultAttributes> &default_emitter = reinterpret_cast<const Emitter<DefaultAttributes> &>(emitter);
        CreateEmitterContextInternal(context, default_emitter, emitter_shader_info);

        {
            std::vector<Particle<ParticleAttrib>> particles(emitter.maxParticleCount);
            glGenBuffers(2, context.ssbo);
            for (int i = 0; i < 2; ++i)
            {
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, context.ssbo[i]);
                glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle<ParticleAttrib>), particles.data(), GL_DYNAMIC_DRAW);
            }
        }

        glGenBuffers(1, &context.emitter_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, context.emitter_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Emitter<ExtraAttrib>), &emitter, GL_STATIC_DRAW);

        m_emitter_contexts.push_back(std::move(context));
    }
    
private:
    struct EmitterContext
    {
        GLuint ssbo[2];
        GLuint emitter_ubo;
        GLuint atomic_buffer;
        ShaderProgram simulate_program;
        ShaderProgram display_program;
        std::vector<Texture> textures;
        std::vector<int> texture_bindings;
        int max_particle_count = 0;
    };

    void CreateEmitterContextInternal(EmitterContext &context, const Emitter<DefaultAttributes> &emitter, const EmitterShaderInfo &emitter_shader_info);
    
    int m_local_size_x = 256; // Match the local_size_x in compute shader
    GLuint m_zero = 0;
    
    GLuint m_ubo_view;
    GLuint m_ssbo_current_binding_points[2];
    std::vector<EmitterContext> m_emitter_contexts;
};
} // namespace Partix
