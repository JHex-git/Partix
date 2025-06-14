#include <Partix/PartixEngine.h>
#include <Partix/Emitter.h>
#include <Partix/View.h>
#include <Partix/Bindings.h>
#include <iostream>

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

        if (context.mesh != nullptr)
        {
            glBindVertexArray(context.vao);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(context.mesh_element_count), GL_UNSIGNED_INT, nullptr, context.max_particle_count);
        }
        else
        {
            glDrawArrays(GL_POINTS, 0, static_cast<int>(context.max_particle_count));
        }
    }
    
    std::swap(m_ssbo_current_binding_points[0], m_ssbo_current_binding_points[1]);
}

void PartixEngine::AddEmitter(const EmitterBase *emitter, int emitter_size, const EmitterShaderInfo& emitter_shader_info, const void *particles, int particles_count, int particle_size)
{
    EmitterContext context;
    const Emitter<DefaultAttributes> &default_emitter = *reinterpret_cast<const Emitter<DefaultAttributes> *>(emitter);
    CreateEmitterContextInternal(context, default_emitter, emitter_shader_info);

    glGenBuffers(2, context.ssbo);
    for (int i = 0; i < 2; ++i)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, context.ssbo[i]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, particles_count * particle_size, particles, GL_DYNAMIC_DRAW);
    }
    
    glGenBuffers(1, &context.emitter_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, context.emitter_ubo);
    glBufferData(GL_UNIFORM_BUFFER, emitter_size, emitter, GL_STATIC_DRAW);

    m_emitter_contexts.push_back(std::move(context));
}

void PartixEngine::CreateEmitterContextInternal(EmitterContext &context, const Emitter<DefaultAttributes> &emitter, const EmitterShaderInfo &emitter_shader_info)
{
    context.max_particle_count = emitter.maxParticleCount;

    glGenBuffers(1, &context.atomic_buffer);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, context.atomic_buffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), &m_zero, GL_DYNAMIC_DRAW);

    {
        Shader simulate_shader(ShaderType::Compute);
        simulate_shader.Load(emitter_shader_info.simulate_shader_path, emitter_shader_info.defines);
        std::vector<Shader> simulate_shaders;
        simulate_shaders.push_back(std::move(simulate_shader));
        context.simulate_program.Load(simulate_shaders);
    }

    if (emitter_shader_info.mesh == nullptr || !emitter_shader_info.mesh->HasLoaded())
    {
        Shader display_vert(ShaderType::Vertex);
        Shader display_geom(ShaderType::Geometry);
        Shader display_frag(ShaderType::Fragment);
        std::vector<Shader> display_shaders;
        display_vert.Load("particle.vert", emitter_shader_info.defines);
        display_geom.Load("particle.geom", emitter_shader_info.defines);
        display_frag.Load(emitter_shader_info.sprite_shader_path, emitter_shader_info.defines);
        display_shaders.push_back(std::move(display_vert));
        display_shaders.push_back(std::move(display_geom));
        display_shaders.push_back(std::move(display_frag));
        context.display_program.Load(display_shaders);
    }
    else
    {
        Shader display_vert(ShaderType::Vertex);
        Shader display_frag(ShaderType::Fragment);
        std::vector<Shader> display_shaders;
        display_vert.Load("mesh.vert", emitter_shader_info.defines);
        display_frag.Load(emitter_shader_info.sprite_shader_path, emitter_shader_info.defines);
        display_shaders.push_back(std::move(display_vert));
        display_shaders.push_back(std::move(display_frag));
        context.display_program.Load(display_shaders);
    }

    assert(emitter_shader_info.sprite_shader_texture_paths.size() == emitter_shader_info.sprite_texture_bindings.size());
    for (int i = 0; i < emitter_shader_info.sprite_shader_texture_paths.size(); ++i)
    {
        Partix::Texture texture;
        texture.Load(emitter_shader_info.sprite_shader_texture_paths[i]);
        context.textures.push_back(std::move(texture));
        context.texture_bindings.push_back(emitter_shader_info.sprite_texture_bindings[i]);
    }

    context.mesh = emitter_shader_info.mesh;
    glGenVertexArrays(1, &context.vao);
    glBindVertexArray(context.vao);
    if (emitter_shader_info.mesh != nullptr && emitter_shader_info.mesh->HasLoaded())
    {
        const std::vector<SubMesh>& submeshes = emitter_shader_info.mesh->GetSubMeshes();
        size_t mesh_count = submeshes.size();
        glGenBuffers(static_cast<GLsizei>(mesh_count), &context.mesh_vbo);
        glGenBuffers(static_cast<GLsizei>(mesh_count), &context.mesh_ebo);
        glBindBuffer(GL_ARRAY_BUFFER, context.mesh_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.mesh_ebo);
        size_t vertex_count = 0;
        size_t index_count = 0;
        for (size_t i = 0; i < mesh_count; ++i)
        {
            vertex_count += submeshes[i].vertices.size();
            index_count += submeshes[i].indices.size();
        }
        context.mesh_element_count = index_count;
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
        GLint vertex_offset = 0;
        GLint index_offset = 0;
        for (size_t i = 0; i < mesh_count; ++i)
        {
            GLsizeiptr vertex_size = submeshes[i].vertices.size() * sizeof(Vertex);
            GLsizeiptr index_size = submeshes[i].indices.size() * sizeof(unsigned int);
            glBufferSubData(GL_ARRAY_BUFFER, vertex_offset, vertex_size, submeshes[i].vertices.data());
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_offset, index_size, submeshes[i].indices.data());
            vertex_offset += static_cast<GLint>(vertex_size);
            index_offset += static_cast<GLint>(index_size);
        }

        glVertexAttribPointer(0, sizeof(Vertex::position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, sizeof(Vertex::tex_coords) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));
        glEnableVertexAttribArray(1);
    }
}
} // namespace Partix
