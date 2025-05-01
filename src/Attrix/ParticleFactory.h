#pragma once
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <tinyxml2.h>
#include <Partix/Particle.h>

namespace Partix
{

struct ParticleAttributeMeta
{
    int offset;
    int size;
    void *instance;
    std::string fields;
};

class ParticleFactory
{
public:
    static ParticleFactory &Instance()
    {
        static ParticleFactory instance;
        return instance;
    }

    ParticleFactory(const ParticleFactory &) = delete;
    ParticleFactory &operator=(const ParticleFactory &) = delete;
    ParticleFactory(ParticleFactory &&) = delete;
    ParticleFactory &operator=(ParticleFactory &&) = delete;

    template <typename ParticleAttrib>
    void RegisterParticle(const std::string &type_name, std::string &&fields)
    {
        const auto meta_iter = m_attribute_metas.find(type_name);
        if (meta_iter == m_attribute_metas.end())
        {
            ParticleAttributeMeta meta;
            meta.offset = offsetof(Particle<ParticleAttrib>, attributes) - sizeof(Particle<DefaultAttributes>);
            meta.size = sizeof(ParticleAttrib);
            meta.instance = new ParticleAttrib;
            meta.fields = std::move(fields);
            m_attribute_metas.insert({type_name, meta});
        }
        else
        {
            std::cerr << "Particle Attribute has already been registered!" << std::endl;
        }
    }

    void CreateParticlesInBuffer(const std::string &type_name, void *buffer, int count);
    void InitializeParticlesSeedInBuffer(const std::string &type_name, void *buffer, int count);

    int GetParticleSize(const std::string &type_name);
    const std::string GetParticleFields(const std::string &type_name);

private:
    ParticleFactory()
    {
        ParticleAttributeMeta default_meta;
        default_meta.offset = 0;
        default_meta.size = sizeof(Particle<DefaultAttributes>);
        default_meta.instance = new Particle<DefaultAttributes>;
        m_attribute_metas.insert({"Default", default_meta});
    }
    ~ParticleFactory() = default;

private:
    std::map<std::string, ParticleAttributeMeta> m_attribute_metas;

    std::random_device m_random_device;
    std::mt19937 m_engine{m_random_device()};
};
} // namespace Partix
