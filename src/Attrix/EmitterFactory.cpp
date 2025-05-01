#include <Partix/Emitter.h>
#include <Attrix/EmitterFactory.h>

namespace Partix
{
EmitterBase *EmitterFactory::CreateEmitter(const std::string &type_name, tinyxml2::XMLElement *emitter_attributes_element)
{
    auto it = m_emitter_constructors.find(type_name);
    if (it != m_emitter_constructors.end())
    {
        return it->second(emitter_attributes_element);
    }
    return nullptr;
}

int EmitterFactory::GetEmitterSize(const std::string &type_name)
{
    auto it = m_emitter_sizes.find(type_name);
    if (it != m_emitter_sizes.end())
    {
        return it->second;
    }
    return 0;
}

void EmitterFactory::RegisterEmitter(const std::string &type_name, int emitter_size, EmitterConstructor&& constructor)
{
    m_emitter_constructors[type_name] = constructor;
    m_emitter_sizes[type_name] = emitter_size;
}
} // namespace Partix
