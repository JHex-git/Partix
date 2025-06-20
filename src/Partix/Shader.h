#pragma once
#include <string>
#include <map>
#include <glad/glad.h>

namespace Partix
{

enum class ShaderType : GLenum
{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Compute = GL_COMPUTE_SHADER
};

class Shader
{
public:
    Shader(ShaderType);
    ~Shader();

    Shader(Shader&&);
    const Shader& operator=(Shader&&);

    Shader(const Shader&) = delete;

    bool Load(const std::string &path, const std::map<std::string, std::string> &defines) const;
    inline unsigned int GetID() const { return m_id; }
private:
    ShaderType m_type;
    unsigned int m_id;
};

} // namespace Partix
