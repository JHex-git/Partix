#pragma once
#include <vector>
#include <glad/glad.h>
#include <Partix/Particle.h>

namespace Partix
{

class ParticleBuffer
{
public:
    ParticleBuffer()
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    bool Init(size_t max_num)
    {
        std::vector<Particle> particles(max_num);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * max_num, particles.data(), GL_DYNAMIC_DRAW);
    }

    ~ParticleBuffer()
    {
        if (m_id != 0)
        {
            glDeleteBuffers(1, &m_id);
        }
    }
private:
     unsigned int m_id;
};
} // namespace Partix
