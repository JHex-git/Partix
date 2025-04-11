#pragma once
#include <string>
#include <glad/glad.h>
namespace Partix
{

class UniformBuffer
{
public:
    UniformBuffer()
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    }

    ~UniformBuffer()
    {
        if (m_id != 0) glDeleteBuffers(1, &m_id);
    }

    void Transfer(const void* data, size_t size) const
    {
        Bind();
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

    void BindBlock(GLuint program, const std::string& block_name, GLuint binding_point)
    {
        GLuint blockIndex = glGetUniformBlockIndex(program, block_name.c_str());
        glUniformBlockBinding(program, blockIndex, binding_point);
        m_binding_point = binding_point;
    }

    void ActivateBindingPoint() const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, m_binding_point, m_id);
    }

    unsigned int GetID() const { return m_id; }
    
private:
    void Bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    }

    void Unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    unsigned int m_id = 0;
    GLuint m_binding_point = 0;
};
} // namespace Partix
