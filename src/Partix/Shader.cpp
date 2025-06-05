#include <fstream>
#include <iostream>
#include <sstream>
#include <Partix/Shader.h>

namespace Partix
{

Shader::Shader(ShaderType type) : m_type(type)
{
    m_id = glCreateShader(static_cast<GLenum>(m_type));
}

Shader::~Shader()
{
    if (m_id != 0) glDeleteShader(m_id);
}

Shader::Shader(Shader&& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_type = other.m_type;
        other.m_id = 0;
    }
}

const Shader& Shader::operator=(Shader&& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_type = other.m_type;
        other.m_id = 0;
    }
    return *this;
}

bool Shader::Load(const std::string &path, const std::map<std::string, std::string> &defines) const
{
    std::ifstream shader_input("Shader/" + path);
    if (!shader_input.is_open())
    {
        std::cerr << "Shader file " << path << " not found" << std::endl;
        return false;
    }

    std::stringstream ss;
    ss << shader_input.rdbuf();
    shader_input.close();

    std::string shader_string = ss.str();
    std::string define_string;
    for (const auto& [key, value] : defines)
    {
        define_string += "#define " + key + " " + value + "\n";
    }

    // Find #version line and insert defines after it
    size_t version_pos = shader_string.find("#version");
    if (version_pos != std::string::npos)
    {
        size_t end_of_version = shader_string.find('\n', version_pos);
        if (end_of_version != std::string::npos)
        {
            shader_string.insert(end_of_version + 1, define_string);
        }
    }
    else
    {
        std::cerr << "No #version line found in shader " << path << std::endl;
        return false;
    }
    
    const char *shader_source = shader_string.c_str();
    glShaderSource(m_id, 1, &shader_source, nullptr);
    glCompileShader(m_id);

    GLint success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        int length;
        glGetShaderInfoLog(m_id, 512, &length, info_log);
        std::cerr << "Failed to compile Shader " << path << std::endl;
        std::cerr << info_log << std::endl;
        return false;
    }

    return true;
}

} // namespace Partix
