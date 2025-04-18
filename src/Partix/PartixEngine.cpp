#include <Partix/PartixEngine.h>
#include <Partix/Emitter.h>
#include <Partix/View.h>
#include <Partix/Bindings.h>
#include <Partix/Particle.h>

namespace Partix
{

PartixEngine::PartixEngine()
{
    glGenBuffers(1, &m_ubo_view);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_view);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(View), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, PARTIX_VIEW_UBO_BINDING_POINT, m_ubo_view);
    m_ssbo_current_binding_points[0] = PARTIX_SSBO_BINDING_POINT0;
    m_ssbo_current_binding_points[1] = PARTIX_SSBO_BINDING_POINT1;
}

void PartixEngine::AddEmitter(const Emitter &emitter, int max_particle_count, const std::string &simulate_shader_path, const std::string &sprite_shader_path, const std::vector<std::string> &sprite_shader_texture_paths, const std::vector<int> &sprite_texture_bindings)
{
    EmitterContext context;
    context.max_particle_count = max_particle_count;

    {
        std::vector<Particle> particles(max_particle_count);
        glGenBuffers(2, context.ssbo);
        for (int i = 0; i < 2; ++i)
        {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, context.ssbo[i]);
            glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_ssbo_current_binding_points[i], context.ssbo[i]);
        }
    }

    glGenBuffers(1, &context.atomic_buffer);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, context.atomic_buffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), &m_zero, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, PARTIX_ATOMIC_BUFFERr_BINDING_POINT, context.atomic_buffer);

    glGenBuffers(1, &context.emitter_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, context.emitter_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Emitter), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, PARTIX_EMITTER_UBO_BINDING_POINT, context.emitter_ubo);

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
    
    glBindBuffer(GL_UNIFORM_BUFFER, context.emitter_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Emitter), &emitter);

    m_emitter_contexts.push_back(std::move(context));
}
    
void PartixEngine::Tick(const View &view)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_view);
    glBufferSubData(GL_UNIFORM_BUFFER, PARTIX_VIEW_UBO_BINDING_POINT, sizeof(View), &view);
    
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    for (const EmitterContext &context : m_emitter_contexts)
    {
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, context.atomic_buffer);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, PARTIX_ATOMIC_BUFFERr_BINDING_POINT, sizeof(GLuint), &m_zero);
    }
    
    for (const EmitterContext &context : m_emitter_contexts)
    {
        // update particles
        context.simulate_program.Bind();
        int num_work_groups = (context.max_particle_count + m_local_size_x - 1) / m_local_size_x;
        glDispatchCompute(num_work_groups, 1, 1);

        // draw previous frame particles
        context.display_program.Bind();
        for (int i = 0; i < context.textures.size(); ++i)
        {
            context.textures[i].Bind(context.texture_bindings[i]);
        }
        glDrawArrays(GL_POINTS, 0, static_cast<int>(context.max_particle_count));
    }
    
    std::swap(m_ssbo_current_binding_points[0], m_ssbo_current_binding_points[1]);
    for (int i = 0; i < 2; ++i)
    {
        for (const EmitterContext &context : m_emitter_contexts)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_ssbo_current_binding_points[i], context.ssbo[i]);
        }
    }
}
} // namespace Partix
