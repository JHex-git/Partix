#pragma once
#include <vector>
#include <glad/glad.h>
#include <Partix/Texture.h>
#include <Partix/ShaderProgram.h>

namespace Partix
{

struct Emitter;
struct View;

class PartixEngine
{
public:
    PartixEngine();

    void AddEmitter(const Emitter &emitter, int max_particle_count, const std::string &simulate_shader_path, const std::string &sprite_shader_path, const std::vector<std::string> &sprite_shader_texture_paths, const std::vector<int> &sprite_texture_bindings);
    
    void Tick(const View &view);
    
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
