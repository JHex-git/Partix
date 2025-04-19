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

    template <typename ExtraAttrib>
    void AddEmitter(const Emitter<ExtraAttrib> &emitter, const std::string &simulate_shader_path, const std::string &sprite_shader_path, const std::vector<std::string> &sprite_shader_texture_paths, const std::vector<int> &sprite_texture_bindings)
    {
        EmitterContext context;
        context.max_particle_count = emitter.maxParticleCount;

        {
            std::vector<Particle> particles(emitter.maxParticleCount);
            glGenBuffers(2, context.ssbo);
            for (int i = 0; i < 2; ++i)
            {
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, context.ssbo[i]);
                glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
            }
        }

        glGenBuffers(1, &context.atomic_buffer);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, context.atomic_buffer);
        glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), &m_zero, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &context.emitter_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, context.emitter_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Emitter<ExtraAttrib>), &emitter, GL_STATIC_DRAW);

        Shader simulate_shader(ShaderType::Compute);
        simulate_shader.Load(simulate_shader_path);
        context.simulate_program.Load({ simulate_shader });

        Shader display_vert(ShaderType::Vertex);
        Shader display_geom(ShaderType::Geometry);
        Shader display_frag(ShaderType::Fragment);
        display_vert.Load("particle.vert");
        display_geom.Load("particle.geom");
        display_frag.Load(sprite_shader_path);
        context.display_program.Load({display_vert, display_geom, display_frag});

        assert(sprite_shader_texture_paths.size() == sprite_texture_bindings.size());
        for (int i = 0; i < sprite_shader_texture_paths.size(); ++i)
        {
            Partix::Texture texture;
            texture.Load(sprite_shader_texture_paths[i]);
            context.textures.push_back(std::move(texture));
            context.texture_bindings.push_back(sprite_texture_bindings[i]);
        }

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
    
    int m_local_size_x = 256; // Match the local_size_x in compute shader
    GLuint m_zero = 0;
    
    GLuint m_ubo_view;
    GLuint m_ssbo_current_binding_points[2];
    std::vector<EmitterContext> m_emitter_contexts;
};
} // namespace Partix
