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

bool Shader::Load(const std::string &path) const
{
    std::ifstream shader_input(path);
    if (!shader_input.is_open())
    {
        std::cerr << "Shader file " << path << " not found" << std::endl;
        return false;
    }

    std::stringstream ss;
    ss << shader_input.rdbuf();
    shader_input.close();

    std::string shader_string = ss.str();
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
