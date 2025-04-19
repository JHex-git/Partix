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

    void AddEmitter(const EmitterBase *emitter, int emitter_size, const std::string &simulate_shader_path, const std::string &sprite_shader_path, const std::vector<std::string> &sprite_shader_texture_paths, const std::vector<int> &sprite_texture_bindings)
    {
        EmitterContext context;
        const Emitter<DefaultAttributes> &default_emitter = *reinterpret_cast<const Emitter<DefaultAttributes> *>(emitter);
        CreateEmitterContextInternal(context, default_emitter, simulate_shader_path, sprite_shader_path, sprite_shader_texture_paths, sprite_texture_bindings);

        glGenBuffers(1, &context.emitter_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, context.emitter_ubo);
        glBufferData(GL_UNIFORM_BUFFER, emitter_size, emitter, GL_STATIC_DRAW);

        m_emitter_contexts.push_back(std::move(context));
    }

    template <typename ExtraAttrib>
    void AddEmitter(const Emitter<ExtraAttrib> &emitter, const std::string &simulate_shader_path, const std::string &sprite_shader_path, const std::vector<std::string> &sprite_shader_texture_paths, const std::vector<int> &sprite_texture_bindings)
    {
        EmitterContext context;

        const Emitter<DefaultAttributes> &default_emitter = reinterpret_cast<const Emitter<DefaultAttributes> &>(emitter);
        CreateEmitterContextInternal(context, default_emitter, simulate_shader_path, sprite_shader_path, sprite_shader_texture_paths, sprite_texture_bindings);

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

    void CreateEmitterContextInternal(EmitterContext &context, const Emitter<DefaultAttributes> &emitter, const std::string &simulate_shader_path, const std::string &sprite_shader_path, const std::vector<std::string> &sprite_shader_texture_paths, const std::vector<int> &sprite_texture_bindings);
    
    int m_local_size_x = 256; // Match the local_size_x in compute shader
    GLuint m_zero = 0;
    
    GLuint m_ubo_view;
    GLuint m_ssbo_current_binding_points[2];
    std::vector<EmitterContext> m_emitter_contexts;
};
} // namespace Partix
