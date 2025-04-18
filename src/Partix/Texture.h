#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

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

    bool Load(const std::string &path)
    {
        int width;
        int height;
        int channels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (data == nullptr)
        {
            std::cerr << "Failed to load texture: " << path << std::endl;
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return true;
    }

    void Bind(GLuint unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

private:
    GLuint m_id = 0;
};
} // namespace Partix
