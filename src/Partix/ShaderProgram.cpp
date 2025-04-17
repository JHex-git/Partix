#include <iostream>
#include <glad/glad.h>
#include <Partix/ShaderProgram.h>

namespace Partix
{

ShaderProgram::ShaderProgram()
{
    m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    if (m_id != 0) glDeleteProgram(m_id);
}

bool ShaderProgram::Load(const std::vector<Shader> &shaders) const
{
    for (const Shader& shader : shaders)
    {
        glAttachShader(m_id, shader.GetID());
    }
    glLinkProgram(m_id);

    GLint success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLsizei length;
        GLchar info[512];
        glGetProgramInfoLog(m_id, 512, &length, info);
        std::cerr << info << std::endl;
        return false;
    }

    return true;
}

void ShaderProgram::SetParam(const std::string &param, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, param.c_str()), value);
}

void ShaderProgram::SetParam(const std::string &param, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, param.c_str()), 1, GL_FALSE,  &value[0][0]);
}

void ShaderProgram::Bind()const
{
    glUseProgram(m_id);
}

} // namespace Partix
