#include <Partix/PartixEngine.h>
#include <Partix/Emitter.h>
#include <Partix/View.h>
#include <Partix/Bindings.h>

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

void PartixEngine::Tick(const View &view)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_view);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(View), &view);
    
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    for (const EmitterContext &context : m_emitter_contexts)
    {
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, context.atomic_buffer);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &m_zero);
    }
    
    for (const EmitterContext &context : m_emitter_contexts)
    {
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, PARTIX_ATOMIC_BUFFERr_BINDING_POINT, context.atomic_buffer);
        glBindBufferBase(GL_UNIFORM_BUFFER, PARTIX_EMITTER_UBO_BINDING_POINT, context.emitter_ubo);
        for (int i = 0; i < 2; ++i)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_ssbo_current_binding_points[i], context.ssbo[i]);
        }

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
}
} // namespace Partix
