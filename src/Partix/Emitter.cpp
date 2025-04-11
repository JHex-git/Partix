#include <Partix/Emitter.h>

namespace Partix
{

void Emitter::BuildData()
{
    for (auto& component : m_components)
    {
        component->BuildData(m_context);
    }
}

void Emitter::Bind(unsigned int &bind_point)
{
    m_context.BindProgram(m_program.GetID(), bind_point);
}

bool Emitter::Compile(const std::string shader_path)
{
    Shader shader(ShaderType::Compute);
    bool successful = shader.Load(shader_path);
    if (successful)
    {
        successful = m_program.Load({shader});
    }

    return successful;
}
} // namespace Partix
