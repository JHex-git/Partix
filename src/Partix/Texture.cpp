#include <iostream>
#include <stb_image.h>
#include <Partix/Texture.h>

namespace Partix
{

bool Texture::Load(const std::string &path)
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
    GLenum format = channels == 4 ? GL_RGBA : 
                    channels == 3 ? GL_RGB : GL_RED; // Fallback to GL_RED if channels is not 3 or 4
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return true;
}

void Texture::Bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

} // namespace Partix
