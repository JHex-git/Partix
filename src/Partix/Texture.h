#pragma once
#include <string>
#include <glad/glad.h>

namespace Partix
{

class Texture
{
public:
    Texture()
    {
        glGenTextures(1, &m_id);
    }

    ~Texture()
    {
        if (m_id != 0)
        {
            glDeleteTextures(1, &m_id);
        }
    }

    Texture(const Texture &) = delete;

    Texture(Texture && other)
        : m_id(0)
    {
        std::swap(m_id, other.m_id);
    }

    Texture &operator=(Texture &&other) = delete;

    bool Load(const std::string &path);

    void Bind(GLuint unit) const;

private:
    GLuint m_id = 0;
};
} // namespace Partix
