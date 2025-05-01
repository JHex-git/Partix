#include <Attrix/ParticleFactory.h>

namespace Partix
{

void ParticleFactory::CreateParticlesInBuffer(const std::string &type_name, void *buffer, int count)
{
    const ParticleAttributeMeta &default_meta = m_attribute_metas["Default"];
    if (type_name == "Default")
    {
        for (int i = 0; i < count; ++i)
        {
            memcpy((char *)buffer + sizeof(Particle<DefaultAttributes>) * i, default_meta.instance, default_meta.size);
        }
        return;
    }

    const ParticleAttributeMeta &meta = m_attribute_metas[type_name];
    for (int i = 0; i < count; ++i)
    {
        memcpy((char *)buffer + (sizeof(Particle<DefaultAttributes>) + meta.offset + meta.size) * i, default_meta.instance, default_meta.size);
        memcpy((char *)buffer + (sizeof(Particle<DefaultAttributes>) + meta.offset + meta.size) * i + meta.offset, meta.instance, meta.size);
    }
}

void ParticleFactory::InitializeParticlesSeedInBuffer(const std::string &type_name, void *buffer, int count)
{
    std::uniform_int_distribution<unsigned int> seed_distribution(0, 10000);

    const ParticleAttributeMeta &meta = m_attribute_metas[type_name];
    for (int i = 0; i < count; ++i)
    {
        unsigned int *seed = (unsigned int *)((char *)buffer + (sizeof(Particle<DefaultAttributes>) + meta.offset + meta.size) * i + offsetof(Particle<DefaultAttributes>, seed));
        for (int i = 0; i < 4; ++i)
        {
            seed[i] = seed_distribution(m_engine);
        }
    }
}

int ParticleFactory::GetParticleSize(const std::string &type_name)
{
    auto it = m_attribute_metas.find(type_name);
    if (it != m_attribute_metas.end())
    {
        return sizeof(Particle<DefaultAttributes>) + it->second.offset + it->second.size;
    }
    return 0;
}

const std::string ParticleFactory::GetParticleFields(const std::string &type_name)
{
    auto it = m_attribute_metas.find(type_name);
    if (it != m_attribute_metas.end())
    {
        return it->second.fields;
    }
    return "";
}

} // namespace Partix
